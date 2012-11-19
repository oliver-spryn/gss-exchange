#include <Windows.h>
#include <sqlext.h>
#include <string>
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


#include <vector>

using std::string;
using std::vector;

class Database {
public : 
	Database(char localServerName[]);
	Database(char server[], char port[], char database[], char userID[], char password[]);
	~Database();
	vector<vector<string>> query(string query);

protected : 
	HANDLE cHandle;
	string displayError(unsigned int handleType, const SQLHANDLE &handle, string customMessage);
	HANDLE eHandle;
	HANDLE sHandle;
	SQLRETURN sReturn;
};

typedef struct {
public : 
	int columnPosition;
	SQLCHAR columnName[128];
	SQLSMALLINT columnType;
	SQLULEN columnSize;
} ColumnInfo;

typedef struct {
public : 
	SQLCHAR SQLChar[100000];
	//SQLVARCHAR SQLVarchar[100000];
	//SQLNUMERIC SQLNumeric;
	//SQLDECIMAL SQLDecimal;
	SQLINTEGER SQLInteger;
	SQLSMALLINT SQLSmallInt;
	SQLFLOAT SQLFloat;
	SQLREAL SQLReal;
	//SQLDOUBLE SQLDouble;
	SQL_TIMESTAMP_STRUCT SQLDate;
	SQLTIME SQLTime;
	SQLTIMESTAMP SQLTimestamp;

	SQLINTEGER SQLLength;
} ColumnTypes;