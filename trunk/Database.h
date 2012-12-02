#include <Windows.h>
#include <map>
#include <sqlext.h>
#include <string>
#include <vector>

using std::map;
using std::string;
using std::vector;

class Database {
public : 
	Database(char localServerName[], string database);
	Database(char server[], char port[], char database[], char userID[], char password[]);
	~Database();

	vector<map<string, string>> query(string query);
	vector<map<string, string>> sQuery(string query, char** parameters);

	vector<string> attributes;
	int size;

protected : 
	HANDLE cHandle;
	string displayError(unsigned int handleType, const SQLHANDLE &handle, string customMessage);
	HANDLE eHandle;
	HANDLE sHandle;
	SQLRETURN sReturn;

private : 
	vector<map<string, string>> extract();
};

typedef struct {
public : 
	int columnPosition;
	string columnName;
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

typedef vector<map<string, string>> SQLData;