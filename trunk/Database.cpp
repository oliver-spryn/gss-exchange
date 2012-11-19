/* 
  Author:  Oliver SPryn
  Course:  COMP 244, Database Management
  Date:    11 October 2012 
  Description:  This file is a wrapper for all of the functionality
                required to connect and query the SQL server. It provides
				multiple methods of connecting to a server via ODBC and
				will determine the number and types of each	returned 
				attribute.
*/ 

#include <iostream>
#include <limits>
#include <Windows.h>
#include <sqlext.h>
#include <sstream>
#include <string>
#include <vector>

#include "Database.h"
#include "DatabaseAttributeTypeUnknown.h"
#include "DatabaseConnectionException.h"
#include "DatabaseQueryFailedException.h"

using std::cout;
using std::endl;
using std::string;
using std::stringstream;
using std::vector;

Database::Database(char localServerName[]) {
//Allocate the pre-connection handles
	this->sReturn = SQL_SUCCESS;
	this->sReturn = SQLAllocHandle(SQL_HANDLE_ENV, SQL_NULL_HANDLE, &this->eHandle);
	this->sReturn = SQLSetEnvAttr(this->eHandle, SQL_ATTR_ODBC_VERSION, reinterpret_cast<void*>(SQL_OV_ODBC3), 4);
	SQLAllocHandle(SQL_HANDLE_DBC, this->eHandle, &this->cHandle);

//Connect to the database
	this->sReturn = SQLConnect(this->cHandle, reinterpret_cast<SQLCHAR*>(localServerName), SQL_NTS, NULL, 0, NULL, 0);

//Check to see if the connection was successful
	if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
		//Success
	} else {
		throw DatabaseConnectionException(this->displayError(SQL_HANDLE_DBC, this->cHandle,
			"Connection to the database was unsuccessful. Double check the name of the local ODBC connection to the server."));
	}
}

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

//Connect to the database
	SQLCHAR returnConection[1024];
	SQLCHAR* logon = reinterpret_cast<SQLCHAR*>(connectionString);
	this->sReturn = SQLDriverConnect(this->cHandle, NULL, logon, SQL_NTS, returnConection, 1024, NULL, SQL_DRIVER_NOPROMPT);

//Check to see if the connection was successful
	if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
		//Success
	} else {
		throw DatabaseConnectionException(this->displayError(SQL_HANDLE_DBC, this->cHandle,
			"Connection to the database was unsuccessful. Double check the address and port of the server, name of the database, and logon credentials."));
	}
}

Database::~Database() {
	SQLDisconnect(this->cHandle);
	SQLFreeHandle(SQL_HANDLE_DBC, this->cHandle);
	SQLFreeHandle(SQL_HANDLE_ENV, this->eHandle);
}

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

vector<vector<string>> Database::query(string query) {
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

//Walk over each colum in the returned object and determine its name, data type, and buffer length
	vector<ColumnInfo> info;

	for (int i = 0; i < columnCount; i++) {
		ColumnInfo cInfo;
		cInfo.columnPosition = i + 1;

		SQLDescribeCol(this->sHandle, i + 1, cInfo.columnName, 100, NULL, &cInfo.columnType, &cInfo.columnSize, NULL, NULL);
		info.push_back(cInfo);
	}	
	
//Fetch each of the tuples one attribute at a time
	ColumnTypes type;
	stringstream sin;
	string coercion;
	vector<vector<string>> data;

//Dynamic SQL reading: http://www.anaesthetist.com/mnm/sql/odbc.htm
//ODBC numeric attribute type to C++ type maping: http://www.cplusplus.com/forum/general/36365/
//and http://www.cplusplus.com/forum/general/36840/
//SQL type to C++ type: http://msdn.microsoft.com/en-us/library/windows/desktop/ms710150(v=vs.85).aspx
	while (this->sReturn == SQLFetch(this->sHandle)) {
	//Ensure this operation was executed successfully
		if (this->sReturn == SQL_SUCCESS || this->sReturn == SQL_SUCCESS_WITH_INFO) {
			//Success
		} else {
			stringstream error;
			error << "An error was encountered while building the SQL return matrix.";

			throw DatabaseQueryFailedException(this->displayError(SQL_HANDLE_STMT, this->sHandle, error.str()));
		}

		vector<string> tuple;

		for (int i = 0; i < columnCount; i++) {
			switch (info[i].columnType) {
				case 0 : //SQL_UNKNOWN_TYPE
					throw DatabaseAttributeTypeUnknown("The database returned an attribute of an unknown type.");
					break;

				case 1 : //SQL_CHAR
				case 2 : //SQL_NUMERIC since it is simply an unsigned char typedef
				case 3 : //SQL_DECIMAL since it is simply an unsigned char typedef
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

				//case 9 : //SQL_DATETIME
				//case 91 : //SQL_TYPE_DATE
				//case 92 : //SQL_TYPE_TIME
				case 93 : //SQL_TYPE_TIMESTAMP
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

			tuple.push_back(sin.str());
			sin.str("");
		}

		data.push_back(tuple);
	}

//Deallocate the statement handle
	SQLFreeHandle(SQL_HANDLE_STMT, this->sHandle);
	
	return data;
}