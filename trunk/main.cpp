#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "Database.h"
#include "DatabaseAttributeTypeUnknown.h"
#include "DatabaseConnectionException.h"
#include "DatabaseQueryFailedException.h"

using std::cout;
using std::endl;

int main(int argc, char *argv[]) {
	try {
		Database db("SQLServer", "PROJECT");
		
		/*vector<map<string, string>> data = db.query("SELECT * FROM Users");
		
		for(int i = 0; i < db.size; i++) {
			cout << data[i]["FName"] << " " << data[i]["LName"]
			<< " has a role of " << data[i]["Role"] << endl;
		}

		cout << db.size;*/
		
		char* stuff[] = {"sprynoj1"};
		db.sQuery("SELECT * FROM Users WHERE UName = ?", stuff);
		cout << db.size;
	} catch (DatabaseAttributeTypeUnknown e) {
		cout << e;
	} catch (DatabaseConnectionException e) {
		cout << e;
	} catch (DatabaseQueryFailedException e) {
		cout << e;
	}

	return 0;
}