#include <Windows.h>
#include <map>
#include <sqlext.h>
#include <sstream>
#include <string>
#include <vector>

#include "Database.h"
#include "DatabaseAttributeTypeUnknown.h"
#include "DatabaseConnectionException.h"
#include "DatabaseQueryFailedException.h"

using std::map;
using std::string;
using std::stringstream;
using std::vector;

/**
 * CONSTRUCTOR
 * 
 * This constructor connects to a local SQL Server instance. The desired 
 * database is selected, and all of the necessary environment and connection 
 * handles are allocated to perform later queries. and allocates the 
 * necessary environment and connection handles to perform later queries.
 *
 * @access public
 * @param  char[]                       localServerName The name of the local server instance
 * @return void
 * @throws DatabaseConnectionException                  Occurs when a database connection was unsuccessful
 * @throws DatabaseQueryFailedException                 Occurs if the given database cannot be selected
*/

Database::Database(char localServerName[], string database) {
//Allocate the pre-connection handles
	this->sReturn = SQL_SUCCESS;
	this->sReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->eHandle);
	this->sReturn = SQLSetEnvAttr(this->eHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 4);
	SQLAllocHandle(SQL_HANDLE_DBC, this->eHandle, &this->cHandle);

//Connect to the server
	this->sReturn = SQLConnect(this->cHandle, reinterpret_cast<SQLCHAR*>(localServerName), SQL_NTS, NULL, 0, NULL, 0);

//Check to see if the connection was successful
	if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
		//Success
	} else {
		throw DatabaseConnectionException(this->displayError(SQL_HANDLE_DBC, this->cHandle,
			"Connection to the database was unsuccessful. Double check the name of the local ODBC connection to the server."));
	}

//Select the database
	this->query("USE " + database);
}

/**
 * CONSTRUCTOR
 * 
 * This constructor connects to a remote SQL Server instance by address and
 * port and uses the provided login credentials for authentication. The 
 * desired database is selected, and all of the necessary environment and 
 * connection handles are allocated to perform later queries.
 *
 * @access public
 * @param  char[]                      server   The the address of the server instance
 * @param  char[]                      port     The port on which the SQL Server is running
 * @param  char[]                      database The name of the desired database
 * @param  char[]                      userID   The user name for authentication
 * @param  char[]                      password The password for authentication
 * @return void
 * @throws DatabaseConnectionException          Occurs when a database connection was unsuccessful
*/

Database::Database(char server[], char port[], char database[], char userID[], char password[]) {
//Allocate the pre-connection handles
	this->sReturn = SQL_SUCCESS;
	this->sReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->eHandle);
	this->sReturn = SQLSetEnvAttr(this->eHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 4);
	SQLAllocHandle(SQL_HANDLE_DBC, this->eHandle, &this->cHandle);

//Construct the C-String resembling the connection specifications
	char connectionString[512] = "DRIVER={SQL Server};SERVER=";
	strcat_s(connectionString, server);
	strcat_s(connectionString, ", ");
	strcat_s(connectionString, port);
	strcat_s(connectionString, ";DATABASE=");
	strcat_s(connectionString, database);
	strcat_s(connectionString, ";UID=");
	strcat_s(connectionString, userID);
	strcat_s(connectionString, ";PWD=");
	strcat_s(connectionString, password);
	strcat_s(connectionString, ";");

//Connect to the server and select the database
	SQLCHAR returnConection[1024];
	SQLCHAR* logon = reinterpret_cast<SQLCHAR*>(connectionString);
	this->sReturn = SQLDriverConnect(this->cHandle, NULL, logon, SQL_NTS, returnConection, 1024, NULL, SQL_DRIVER_NOPROMPT);

//Check to see if the connection was successful
	if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
		//Success
	} else {
		throw DatabaseConnectionException(this->displayError(SQL_HANDLE_DBC, this->cHandle,
			"Connection to the database was unsuccessful. Double check the address and port on the server, name of the database, and logon credentials."));
	}
}

/**
 * DESTRUCTOR
 *
 * This destructor disconnects from the SQL Server and frees the connection and
 * environment handles.
 *
 * @access private
 * @return void
*/

Database::~Database() {
	SQLDisconnect(this->cHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, this->cHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, this->eHandle);
}

/**
 * This member function fetches any error data from the ODBC driver or the 
 * SQL server (depending on the encountered error), then formats, and returns
 * the error in a friendly, human readable format.
 *
 * @access private
 * @param  unsigned int     handleType    The type of handle which encountered an error
 * @param  const SQLHANDLE& handle        A reference to the handle which encountered an error
 * @param  string           customMessage A friendly, descriptive message which is included by the developer at the place of error
 * @return string           message       A detailed, formatted, and all inclusive message on the encountered error
*/

string Database::displayError(unsigned int handleType, const SQLHANDLE &handle, string customMessage) {
//Fetch the error message from C++
	SQLCHAR SQLState[1024];
    SQLCHAR systemMessage[1024];

	SQLGetDiagRec(handleType, handle, 1, SQLState, NULL, systemMessage, 1024, NULL);
	
//Construct a customized error message
	string message;
	message = "Message: ";
	message += reinterpret_cast<char*>(systemMessage);
	message += "\nSQL State: ";
	message += reinterpret_cast<char*>(SQLState);
	message += "\n\n";
	message += customMessage;

	return message;
}

/**
 * This member function is where all of the "query magic" happens. This 
 * function is able to take ANY valid SQL query and return a vector
 * matrix of the all results returned from a SELECT statement. This matrix 
 * will represent the strcture of the returned tuples and their attributes.
 * 
 * It will also put the number of tuples returned from a query in the 
 * public "size" member variable, and the name of each of the returned 
 * attributes in the public "attributes" member variable.
 * 
 * For example, if there is a relation such as this:
 *
 *     +----------------------------------+
 *     |             Users                |
 *     +-------+-------+--------+---------+
 *     | FName | LName | UName  |  PWord  |
 *     +-------+-------+--------+---------+
 *     | John  | Smith | JSmith | ABC123! |
 *     +-------+-------+--------+---------+
 *     | Jane  |  Doe  |  JDoe  | DEF456@ |
 *     +-------+-------+--------+---------+
 *
 * ... a query such as this:
 *
 *     SELECT * FROM Users;
 *
 * ... will allow a user to access the data from the statement, like so:
 *
 *    <vector<map<string, string>> data = db.query("SELECT * FROM Users");
 *    cout << data[0]["FName"] << " " << data[0]["LName"]; // John Smith
 *    cout << data[1]["UName"]; // JDoe
 * 
 * This function will return data from tuples containing attributes of the
 * following types:
 *  - BIT
 *  - CHAR(n)
 *  - DECIMAL(p, s)
 *  - DOUBLE
 *  - FLOAT(p)
 *  - INT/INTEGER
 *  - NUMERIC(p, s)
 *  - REAL
 *  - SMALLINT
 *  - TIMESTAMP(p)
 *  - VARCHAR(n)
 *
 * Here are some helpful resources used during the construction of this
 * member function:
 *  - http://www.anaesthetist.com/mnm/sql/odbc.htm
 *  - http://www.cplusplus.com/forum/general/36365/
 *  - http://www.cplusplus.com/forum/general/36840/
 *  - http://msdn.microsoft.com/en-us/library/windows/desktop/ms710150(v=vs.85).aspx
 *  - http://msdn.microsoft.com/en-us/library/windows/desktop/ms714556(v=vs.85).aspx
 *  - http://msdn.microsoft.com/en-us/library/windows/desktop/ms709280(v=vs.85).aspx
 *  - http://msdn.microsoft.com/en-us/library/windows/desktop/ms716298(v=vs.85).aspx
 *
 * @access public
 * @param  string                       query The query to be executed on the database
 * @return vector<map<string, string>>  data  A vector matrix which resembles the set of tuples returned from a query
 * @throws DatabaseQueryFailedException       Occurs if the SQL server returns an error, usually due to a syntax error
 * @throws DatabaseAttributeTypeUnknown       Occurs if the retrieved data contains an attribute type which this method cannot process
*/

vector<map<string, string>> Database::query(string query) {
//Gather information about the query
	SQLINTEGER queryLength = static_cast<SQLINTEGER>(query.length());

	char *cQueryP = new char[query.length() + 1];
	memcpy(cQueryP, query.c_str(), query.length() + 1);
	SQLCHAR* cQuery = reinterpret_cast<SQLCHAR*>(cQueryP);

//Execute the query
	this->sReturn = SQLAllocHandle(SQL_HANDLE_STMT, this->cHandle, &this->sHandle);
	this->sReturn = SQLExecDirect(this->sHandle, cQuery, queryLength);
	
//Ensure the query was executed successfully
	if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
		//Success
	} else {
		throw DatabaseQueryFailedException(this->displayError(SQL_HANDLE_STMT, this->sHandle,
			"The supplied SQL query contained a syntax error."));
	}

//Count the number of returned attributes
	SQLSMALLINT columnCount;
	SQLNumResultCols(this->sHandle, &columnCount);

//Walk over each attribute in the returned object and determine its name, data type, and buffer length
	vector<string> attributeNames;
	vector<ColumnInfo> info;
	SQLCHAR SQLColName[128];

	for (int i = 0; i < columnCount; i++) {
	//Use ODBC to grab information regarding a specific attribute
		ColumnInfo cInfo;
		cInfo.columnPosition = i + 1;

		SQLDescribeCol(this->sHandle, i + 1, SQLColName, 100, NULL, &cInfo.columnType, &cInfo.columnSize, NULL, NULL);

	//Cast the column name from SQLCHAR -> char -> string
		string sColName(reinterpret_cast<char*>(SQLColName));
		cInfo.columnName = sColName;

	//Push this information onto the attribute information vector
		info.push_back(cInfo);
		attributeNames.push_back(sColName);
	}	
	
//Create an instance of several objects used during the data extraction process
	ColumnTypes type;
	stringstream sin;
	vector<map<string, string>> data;

//Loop over each tuple in the relation
	while (this->sReturn == SQLFetch(this->sHandle)) {
	//Ensure this operation was executed successfully
		if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
			//Success
		} else {
			throw DatabaseQueryFailedException(this->displayError(SQL_HANDLE_STMT, this->sHandle, 
				"An error was encountered while building the SQL return matrix."));
		}

	//Loop over each attribute in the tuple
		map<string, string> tuple;

		for (int i = 0; i < columnCount; i++) {
			switch (info[i].columnType) {
				case 0 : //SQL_UNKNOWN_TYPE
					throw DatabaseAttributeTypeUnknown("The database returned an attribute of an unknown type.");
					break;

				case 1  : //SQL_CHAR
				case -7 : //SQL_BIT since it is simply an unsigned char typedef
				case 2  : //SQL_NUMERIC since it is simply an unsigned char typedef
				case 3  : //SQL_DECIMAL since it is simply an unsigned char typedef
				case 12 : //SQL_VARCHAR since it is simply an unsigned char typedef
					this->sReturn = SQLGetData(this->sHandle, i + 1, SQL_C_CHAR,
						&type.SQLChar, (info[i].columnSize + 1)*sizeof(SQLCHAR), &type.SQLLength); //+1 makes room for \0 (maybe?)
					
					sin << type.SQLChar;
					break;
					
				case 4 : //SQL_INTEGER 
					this->sReturn = SQLGetData(this->sHandle, i + 1, SQL_INTEGER,
						&type.SQLInteger, info[i].columnSize*sizeof(SQLINTEGER), &type.SQLLength);
					
					sin << type.SQLInteger;
					break;
					
				case 5 : //SQL_SMALLINT 
					this->sReturn = SQLGetData(this->sHandle, i + 1, SQL_C_SHORT,
						&type.SQLSmallInt, info[i].columnSize*sizeof(SQLSMALLINT), &type.SQLLength);

					sin << type.SQLSmallInt;
					break;
					
				case 6 : //SQL_FLOAT
				case 8 : //SQL_DOUBLE
					this->sReturn = SQLGetData(this->sHandle, i + 1, SQL_C_DOUBLE,
						&type.SQLFloat, info[i].columnSize*sizeof(SQLFLOAT), &type.SQLLength);
					
					sin << type.SQLFloat;
					break;
			
				case 7 : //SQL_REAL
					this->sReturn = SQLGetData(this->sHandle, i + 1, SQL_C_FLOAT,
						&type.SQLReal, info[i].columnSize*sizeof(SQLREAL), &type.SQLLength);
					
					sin << type.SQLReal;
					break;
				
				//case 9  : //SQL_DATETIME
				//case 91 : //SQL_TYPE_DATE
				//case 92 : //SQL_TYPE_TIME
				case 93   : //SQL_TYPE_TIMESTAMP
					this->sReturn = SQLGetData(this->sHandle, i + 1, SQL_C_TIMESTAMP,
						&type.SQLDate, info[i].columnSize*sizeof(SQL_TIMESTAMP_STRUCT), &type.SQLLength);
					
					sin << type.SQLDate.year << "-" << type.SQLDate.month << "-" << type.SQLDate.day << " "
						<< type.SQLDate.hour << ":" << type.SQLDate.minute << ":" << type.SQLDate.second
						<< type.SQLDate.fraction;
					break;

				default : //SQL_WACKO
					throw DatabaseAttributeTypeUnknown("The database returned an attribute of an unknown type.");
					break;
			}

		//Push the value onto the map
			tuple[attributeNames[i]] = sin.str();
			sin.str("");
		}

		data.push_back(tuple);
	}

//Deallocate the statement handle
	SQLFreeHandle(SQL_HANDLE_STMT, this->sHandle);

//Publicize the number of returned tuples and their attribute names
	this->size = data.size();
	this->attributes = attributeNames;
	
	return data;
}