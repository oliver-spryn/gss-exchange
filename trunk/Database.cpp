#include <Windows.h>
#include <iostream>
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
 * A random (friesian) horse... because we needed it:
 * 
 * ?????IIIII???????????III????IIII????????????????+??????????????????????IIIIIIIII
 * ???????????????????????III???????????????????++++++++?+?????????????????????IIII
 * ???????????????????????????????????????????????+:+?+?+I??I????????????I?I???IIII
 * ??????????????????????????????????????++++++++++++++????????????????????????????
 * ??????????????????????????++++++++++++++++++++++++++++++?++????+?????????+??????
 * +++?+????????????++++++++?++++++++++++++++++++++++++++++++++++++++++++++????????
 * +++++?+?++++?++?++++++++++++M++7??O=++++++++++++++++++++++++++++++++++??????????
 * +++++++++++++++++++++++++++IDMMNNDDMN+++++++=+++++++++++++++++??????????????????
 * ++++++++++++++++++=======?$MMMMDNNDNNN++++++++++++++??+?????????????????????????
 * ++++++++++++======~~====+NMN8NMNDN8ODND8++++???????????????????????????+????????
 * ===========~===========+78NDO8DMNMNMM88ZO8+??????????????????????+??+??+++??????
 * =========++++++++++++++IMDNDN8NNMMNNDO8OZ$ZOO+???????+++++++?????++++++++++?+++?
 * +====+++++++++++++++++INDDDDMMMMMMMMNNMMNDN88DD=++++++++++++++++++++++++++++++?+
 * =+=++++++++++++=+++++?ZNMN8DMNMMMMMMMMMDN8OONMMM+=++++++++++++++++++++++++??+???
 * ++===++=========+==+?$DMDN8NMMMMMMMMMMMMMMMDDMMM=++++++++++++++++???????++++++??
 * ================++=+I7O8DMD8NMMDMMMMMM$MMMMNMMM=+++++++??????????????++++++++???
 * ====================Z?MNDDNMMMMMMMMNMMM8N+++++?????????7$DODDMM88MMDN?+?????????
 * ===============+++?+Z$MMDMMMMMMMMMMMMMMMM?++?????????NNMMMNDNMNMMN8?+8O+?????+++
 * ==================+?INMNNMMMMMMMMMMMMMMMMN??????????MMMMM?++MMMMMMMMM+++++++++++
 * ================+++7D$NMMMMMMMMMMMMMMNMMMMMM??+++++MMMM+++++++?I++++++++++++++++
 * ==========+++++++++77MMMMMMNMMMMNMMNMMMMMMMMMM?+++DMMMM+++++++++++++++++++++++++
 * =======+++++++++++?ZMMMMMMMMMMMMMMMMMNNNMMMMMMMMMOMMNN+=+=+==+++======++++++++++
 * ==========+++++++?$MMOMMMMMMMMMMMMMMMMMDDZOOONMMMMMNNM=======+==++++++++++++++++
 * ==========+=+$7I$IIM7MMMNMMMNNOZZZ$ZZ8DDDOZZ$O8NMMMNMM====+==+++++++++++++++++++
 * ==========+======$OIDNMMMMMMND88N8OZ$7$Z$Z8O8DO8MDMMMM====++++++++++==++===+++++
 * ~~~~~~~~~~~~~?=I8~==N~MMMMMMND8N8ZOZZ8NNNMMMNNNNNMMMM+==========================
 * ~~~~~~~~~=I?++=====O+?7MMMMMMNNDND8888DNMNMNMMMMMMMMM?===~=========~=====~==~~~~
 * ==~~~=~~====~~~~+?7=+=IOMMMMMNNMMDD8OO8NNNNNMNNMMMMZZZ8ODNNN~~~~~~~~~~~~~~~~~===
 * ===~~~~=====~~~=+~~~~=?$MMMMMNMMNOOO8ZO8OO8ODNNZZDNNMMMNNNMM7~=============+=+==
 * =~~~~~~~~~~~~~~=+~~~~++7M8DMMNMMNDOOZOOO8ODNNMMNMMMMMMNM=MMM====================
 * ~~~~~~~=~~~~~~+:~~~~=~=?I8$8?MMNN8OODDNDDNMMMMMMNMMMM====MMM====================
 * =====~=~~~~~~==========I+=ZZNMMNNDO8NNMMMMMMMMMMMMM~=====NMM====================
 * ============================MMMNDDZODMMMMMMMMMMMM======?DM8M====================
 * ===========================NMMMD8OODNMMMMMMMMMMMN=====$MDDDN====================
 * ==========================NMMNNDZOODNNNMMNMMMMMM======NMMMNM====================
 * ========================+NMMNNOZOZDDNMMMMMMMMMM========ONMM=====================
 * =======================IMMMMDZZO8DNMMMMMMMMMMM~========NMMM=====================
 * =====================~MMMMM8ZZDNNMMMMMMMMMMMM====~~~~~~~MDN~=~==============~=~=
 * ========~===========7MMMMMNDZDNMMMMMMMMMMMMN~~~~~~~~~~~~~~~~~=~~~~~~~~~~=~~~~~~~
 * ==~==========~~~~~~NNNODNNN8$8NMMMMMMMMMMMZ~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~~~NDDNDDMMMO8NNMMMMMMMMMM~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~
 * ~~~~~~~~~~~~~~~~:DDOOO88MMMMMMMMMMMMMMMM~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~=7?~
 * ~~~~~~~~~~~~~~~~DD8O8ZZZDMMMMMMMMMMMMMM7=~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~+ZZ$88O8
 * ~~~~~~~~~~~~~~~~NDZDNN8ODDMMMMMMMMMMM8I888ZOO~~~~~~~~~=~~~~~~~~~~~~~~~~~?7$+?=8+
 * ~~~~~~~~~~~~~~~DND8NMMNNDD8MNMMMMMMM$+=++==++=~~~~~~~?$Z8I~~~~~~~~~~~~~~O?+Z+??~
 * ~~~~~~~~~~~~~~~NNDDNMMMMNN88MMMMMM~?~=II=~=O~~~~~~~~?78==+~~~~~~~~~~~~~~ZOO$ZZ8D
 * ~~~~~~~~~~~~~~~NM88MNMMMMN8ZDMMMMM8OI?O$O$?=IOD?~~~$IZ$$OZ$8~=+~~~~~~~+~==77OI8Z
 * ~~~~~~~~~~=DMMMNNNZNNMMNNDD8ZNMMMMZ+$Z8OOOZ+=++=~~~+O7O7DZDZ8$O~~~~~~~Z~~??88ZZ8
 * ~~~~~=~~?+DNNNMZMN88DMMNNDDDOZDM+88ZD8O88D==7D8D~~~+$8N7?O==ZZNI~~~~=88ZZ==7?ZON
 * 8O8O8888DNMNNMM8NMN8MMMNNNNND8OMDOD8OO8DN$OZ$7777I7$DDNOZ7Z8NDDD$Z778NMN87ZD8NND
 * 88DN8NODNNNNMMMDZD8NMNMNNNMMNNNM8DDNO8MDDD$8DOOOOZ88ND8ND8O8DDDN88OZNNDN8DD8DDD8
 * D8DDDO8DMNMMMMMD8NMMMMNNNMMMMNMM$ZO87D88OOZO$7$Z$Z$77Z$$Z$$Z7$I777I??III7I??7I?I
 * +?+?+++IMMMMMMND++??MMMMMMMMMMM+??++??I?++?????????III????I???IIIIIIIIIII7?IIII?
 * IIII?IIMMMMMMMMN?II??IMMMMMMNMM?7IIII????+?++????I??II???II7IIII?I?I7II?II???III
 * ??II?IZMMMMDMMMOO???IIIMMNMMMMI+??III?I?IIIII7IIIIII7$7III7II7II7$I777IIIIIII7I7
 * I?IIIIIMMMMNNMMNDZII777MMMMMMMI77$IIIII+I7II7IIIII7$$$$77II7II77I$77I77I7I77I777
 * I7$77$8MMMMMNMMN$$O77$MMMMMMMM7$7$777I777I=I?II77I777III77I$7II7II?I777I$7O777$7
 * II7$$I7MMMMMMMMNO$$7I8MMMMMMMO777I7I?II77I?7IIII?777$?I?7777777$$$$7$$7I?7?I7777
 * $77$77$MMMMMMMMM$7$7DN8MMNMMMI?7777I77I77$7II77Z77??7I77I7IZ7Z$$$$ZIIZ7777I7$777
 * I77III$MMMMNMMMM87$$MMDNNNOMMIII777$I77Z7777$777$7III7777$$$$Z$7$$Z7IIII7?777$7$
 * 77III7MMMMMMMMNMD$ZZZMMMMMMMN7I?$77777$77$7$ZIZ777Z+$I7IIII$Z$$7$II7777777I77$O7
 * I7I77NMMMMMMMDDOO7Z$$MMMMMMMM$IO$$7ZZZ$77IZ$ZIZ$II7I7I??I?II7ZOO8$$?I7$7$7$IZZI?
 * 7Z77DMMMMMNMDDD8Z88$7ZMMMMMMMDZ7ZZZ$ZZZ$7Z+$$$I7I$IIIIIII+??I87DNII8$$7$II7$$O$?
 * I7ONMMNMMMMO7D?N7IOZ7$8MNMZMMM8Z8DD$8D$77ZZZ7Z$ZOIII7$I77$I777?7Z7$II$7$7Z?$?I7Z
 * I7NMNMDMZ?Z8DZM7=877Z7ZODMN8DNNMNM$DNOZ$77?I$IOD7Z7I$777$O7+$$???=+77O?77$77II??
 * 8MMMMMM$DZ?+I+OO??7II7$ON78$M87DNMDNZ7$7II?I$Z77$?77$$I$7$I7=?I77???Z7I7$7I+7I$I
 * 8NM8MN$$I$7?7?7ZI7I?7I7?I$$?Z7Z7$D7?$ZIZZ$7I7?$?+?I$O7O7II77+??I$~7777I?III$?II?
 * 7I??I7??II77III7I7IZZO$Z$8$7IOZ7$?7Z777I77$Z?I7II$7$O8ZO$7+7I??7?$=IO7IIIO+++I$$
 * +I77+$77II7$$~7I$II7I$?7??+$7I7$IIIO?7IZ$77?Z8I$$$77$$?O$Z$7OZI$7=7??O=$$I7=O$=?
 * IO877I7$$7$I77=77$?I7?78?$?$7I7?I=I7$77D?O8II+?8ZOZZ$$7IZ?Z?IO?I7O$?$$OOI7?Z7+$I
 * 77+7$7Z$I?I7$+777$:I?$$IZD$OI~7I?DI?Z+ZI$I$$I8IIZ88OZZZ$II7+ZZ$I7?Z88$77=Z7778$O
 * $ZZIZO8IZI??+Z$I$I7I7?8DOZ$8O=O$:IZZ$$$IZ$I?I$$+7$ZI7IZ7$Z$ZD7$$$?Z7?IOO8Z?$$,,=
 *                                                                   GlassGiant.com
*/

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
 * @access public
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
 * @access protected
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
 * This is the non-safe variation of the sQuery() member function.
 * 
 * This member function is where all of the "query magic" happens. This 
 * function is able to take ANY valid SQL query and return a vector
 * matrix of the all results returned from a SELECT statement. This matrix 
 * will represent the strcture of the returned tuples and their attributes.
 *
 * The purpose of this memeber function over sQuery() is to avoid its 
 * additional overhead. This member function is less expensive than the 
 * sQuery() function, and is recommended only when a query is constructed 
 * in such a way that SQL injection attacks are not possible, such as if
 * an SQL query is compiled directly into an application.
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
 *     SELECT * FROM Users
 *
 * ... will allow a user to access the data from the statement, like so:
 *
 *    SQLData data = db.query("SELECT * FROM Users");
 *    cout << data[0]["FName"] << " " << data[0]["LName"]; // John Smith
 *    cout << data[1]["UName"]; // JDoe
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

	delete cQuery;
	cQueryP = NULL;
	cQuery = NULL;
	
//Ensure the query was executed successfully
	if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
		//Success
	} else {
		throw DatabaseQueryFailedException(this->displayError(SQL_HANDLE_STMT, this->sHandle,
			"The supplied SQL query contained a syntax error."));
	}

//Extract the data from the query
	return this->extract();
}

/**
 * This is the "safe" variation of the query() member function.
 * 
 * This member function is where all of the "query magic" happens. This 
 * function is able to take ANY valid SQL query and return a vector
 * matrix of the all results returned from a SELECT statement. This matrix 
 * will represent the strcture of the returned tuples and their attributes.
 *
 * The purpose of this memeber function over query() is to prevent any
 * possible SQL injection attacks from a user. This member function is more
 * expensive than the query() function, and is recommended only when a query
 * is constructed from user input.
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
 *     SELECT * FROM Users WHERE LName = ? OR LName = ?
 *
 * ... and an array of parameters such as this:
 *
 *     char* params[] = {"Smith", "Doe"};
 *
 * ... will allow a user to access the data from the statement, like so:
 *
 *    SQLData data = db.query("SELECT * FROM Users WHERE LName = ? OR LName = ?", params);
 *    cout << data[0]["FName"] << " " << data[0]["LName"]; // John Smith
 *    cout << data[1]["UName"]; // JDoe
 *
 * Here are some helpful resources used during the construction of this
 * member function:
 *  - http://www.raima.com/docs/rdme/9_1/Content/SQL/SQLDescribeParam.htm
 *  - http://msdn.microsoft.com/en-us/library/windows/desktop/ms710188(v=vs.85).aspx
 *  - http://msdn.microsoft.com/en-us/library/windows/desktop/ms710963(v=vs.85).aspx
 *
 * @access public
 * @param  string                       query      The query to be executed on the database
 * @param  char**                       parameters An array of C-strings which will replace each "?" in the query parameter
 * @return vector<map<string, string>>  data       A vector matrix which resembles the set of tuples returned from a query
 * @throws DatabaseQueryFailedException            Occurs if the SQL server returns an error, usually due to a syntax error
 * @throws DatabaseAttributeTypeUnknown            Occurs if the retrieved data contains an attribute type which this method cannot process
*/

vector<map<string, string>> Database::sQuery(string query, char** parameters) {
//Allocate the statement handle
	this->sReturn = SQLAllocHandle(SQL_HANDLE_STMT, this->cHandle, &this->sHandle);

//Gather information about the query
	SQLINTEGER queryLength = static_cast<SQLINTEGER>(query.length());
	
	char *cQueryP = new char[query.length() + 1];
	memcpy(cQueryP, query.c_str(), query.length() + 1);
	SQLCHAR* cQuery = reinterpret_cast<SQLCHAR*>(cQueryP);

//Run ODBC's preparation function
	this->sReturn = SQLPrepare(this->sHandle, cQuery, queryLength);

	delete cQuery;
	cQueryP = NULL;
	cQuery = NULL;

//Count the number of returned attributes
	SQLSMALLINT paramCount;
	SQLNumParams(this->sHandle, &paramCount);

	if (paramCount) {
	//Walk over each parameter in the query and determine its data type, size, and number of decimal digits
		SQLSMALLINT dataType;
		SQLSMALLINT decimalDigits;
		SQLUINTEGER paramSize;
		SQLINTEGER NTS = SQL_NTS; //SQL_NTS means the data is terminated by a "\0"

		for(int i = 0; i < paramCount; i++) {
		//Use ODBC to grab information regarding a specific parameter
			this->sReturn = SQLDescribeParam(this->sHandle, i + 1, &dataType, &paramSize, &decimalDigits, NULL);
			
		//Bind to and populate this parameter
			this->sReturn = SQLBindParameter(this->sHandle, i + 1, SQL_PARAM_INPUT, SQL_C_CHAR, dataType,
				paramSize, decimalDigits, parameters[i], strlen(parameters[i]), &NTS);
		}
	}

//Execute the prepared query
	this->sReturn = SQLExecute(this->sHandle);

//Extract the data from the query
	return this->extract();
}

/**
 * This function is used to extract the low-level data fetched from the
 * SQL query and transform it into a vector mapping of strings to strings
 * for easy and quick access from the application-level implementation.
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
 * @access private
 * @return vector<map<string, string>>  data A vector matrix which resembles the set of tuples returned from a query
 * @throws DatabaseQueryFailedException      Occurs if the SQL server returns an error, usually due to a syntax error
 * @throws DatabaseAttributeTypeUnknown      Occurs if the retrieved data contains an attribute type which this method cannot process
*/

vector<map<string, string>> Database::extract() {
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
				"An error was encountered while building the SQL return matrix. Perhaps the supplied SQL query contained a syntax error."));
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