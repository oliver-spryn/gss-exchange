#include <iostream>
#include <map>
#include <string>
#include <vector>

#include "UI Admin.h"
#include "UI Display.h"
#include "UI User.h"
#include "Database.h"
#include "DatabaseAttributeTypeUnknown.h"
#include "DatabaseConnectionException.h"
#include "DatabaseQueryFailedException.h"
using std::cout;
using std::endl;

//Begining of Jon's section
bool user_exist(Database* db, string u_name){
	vector<map<string, string>> db_return = db->query("select Count(UName) from Users where UName = '" + u_name + "';");
	if(db_return[0].begin()->second == "0") return false;
	else return true;
}

bool add_user(Database* db, string f_name, string l_name, string u_name, string password){
	string q = "INSERT INTO Users VALUES((select count(UserID) from Users) + 1, ";
	q = q + "'" + f_name + "', ";
	q = q + "'" + l_name + "', ";
	q = q + "'" + u_name + "', ";
	q = q + "'" + password + "', ";
	q+="'User'";
	q+=");";
	if(!user_exist(db, u_name)){
		db->query(q);
	}
	else return false;
}

bool login(Database* db, string u_name, string p_word){
	string q = "select PWord from Users where UName = '"+u_name+"';";
	//cout<<q<<endl;
	vector<map<string, string>> result = db->query(q);
	if(result.size()) return true;
	else return false;
}

bool book_exists(Database* db, string isbn10){
	string q = "select ISBN10 from Books where ISBN10 = '" + isbn10 + "';";
	vector<map<string, string>> result = db->query(q);
	if(result.size()) return true;
	else return false;
}

vector<string> get_book_info(Database* db, string isbn10){
	vector<string> return_results;
	if(book_exists(db, isbn10)){
		vector<map<string, string>> result;
		string q = "select * from Books where ISBN10 = '" + isbn10 + "';";
		result = db->query(q);
		for(map<string, string>::iterator i=result[0].begin(); i!=result[0].end(); i++){
			return_results.push_back(i->second);
		}
		result.clear();
		q = "select Author from Authors where Book = '" + return_results[0] + "';";
		result = db->query(q);
		return_results.push_back(result[0].begin()->second);
	}
	return return_results;
}

void create_new_book(Database* db, string u_name, string isbn10, string isbn13, string title, string edition, string author, string course_code, string price, string condition, bool written, string comment){
	string w = "0";
	if(written) w = "1";
	vector<map<string, string>> result = db->query("select BookID from Books");
	int book_id = result.size()+1;
	result.clear();
	char *c = new char;
	char *c3 = new char;
	char *c5 = new char;
	itoa(book_id, c, 10);
	string c2 = c;
	string q = "INSERT INTO BOOKS VALUES(";
	q = q + "'" + c2 + "', ";
	q = q + "'" + isbn10 + "', ";
	q = q + "'" + isbn13 + "', ";
	q = q + "'" + title + "', ";
	q = q + "'" + edition + "');";
	db->query(q);
	result = db->query("select EntryID from Authors");
	int entry_id = result.size() + 1;
	itoa(entry_id, c3, 10);
	string c4 = c3;
	q = "";
	q = "INSERT INTO AUTHORS VALUES(";
	q = q + "'" + c4 + "', ";
	q = q + "'" + c2 + "', ";
	q = q + "'" + author + "')";
	db->query(q);
	result.clear();
	result = db->query("select CourseID, Name from Courses where Code = '" + course_code + "';");
	result.clear();
	result = db->query("select SaleID from Sale;");
	int sale_id = result.size()+1;
	itoa(sale_id, c5, 10);
	string c6 = c5;
	result.clear();
	result = db->query("select UserID from Users where UName = '" + u_name + "';");
	string user_id = result[0].begin()->second;
	q = "";
	q = "INSERT INTO Sale VALUES(";
	q = q + "'" + c6 + "', ";
	q = q + "'" + c2 + "', ";
	q = q + "'" + user_id + "', ";
	q = q + "CURRENT_TIMESTAMP" + ", ";
	q +="'0', ";
	q = q + "'" + price + "', ";
	q = q + "'" + condition + "', ";
	q = q + "'" + w + "', ";
	q = q + "'" + comment + "');";
	db->query(q);
}

//End of Jon's secrtion

int main(int argc, char *argv[]) {
	try {
		Database db("SQLServer", "PROJECT");
		
		vector<map<string, string>> data = db.query("SELECT * FROM Users");
		
		for(int i = 0; i < db.size; i++) {
			cout << data[i]["FName"] << " " << data[i]["LName"]
			<< " has a role of " << data[i]["Role"] << endl;
		}

		cout << db.size;
		
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

	Display current_Display;
	current_Display.manage_Display();
	return 0;
}