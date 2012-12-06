#include<iostream>
#include<vector>
#include<string>
#include "Database.h"
using std::cout;
using std::endl;

//Begining of David's secion

//This function is a helper function for the add_user function.  It is not necesarry for the
//UI to call this function
//takes a database pointer and a username as a tring
bool user_exist(Database* db, string u_name){
	vector<map<string, string>> db_return = db->query("select Count(UName) from Users where UName = '" + u_name + "';");
	if(db_return[0].begin()->second == "0") return false;
	else return true;
}
//returns true if the username already exists in the database
//returns false if the username does not exist in the database

//add_user takes a database pointer, first name, last name, username, and password as strings
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
//if the username already is in use then the function returns false
//if the username is not already in use then the new user is created and true is returned
//the same person may sign up for multiple accounts so long as they have a unique username for
//each account.  This is because two different people may share the same name

//takes a database pointer, a user name, and a password
bool login(Database* db, string u_name, string p_word){
	if(user_exist(db, u_name)){
		string q = "select PWord from Users where UName = '"+u_name+"';";
		vector<map<string, string>> result = db->query(q);
		if(result.size()) return true;
		else return false;
	}
	else return false;
}
//if the username exists and the username and password match then true is returned
//if the user does not exist or the password does not match the username then false is returned

//this is a helper function for get_book_info
//takes a database pointer and an isbn10 as a string
bool book_exists(Database* db, string isbn10){
	string q = "select ISBN10 from Books where ISBN10 = '" + isbn10 + "';";
	vector<map<string, string>> result = db->query(q);
	if(result.size()) return true;
	else return false;
}
//returns true if this book is already logged in the database
//returns false if the book does not already exist in the database
//the purpose of this function is to save the user time filling in information for a book that
//already exists in the database

//the purpose of this function is to get information on a book a user is trying to add if it
//already exists to save the user time filling in repetitive information
//the function takes a database pointer and an isbn10 as a string
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
//returns a vector of type string with book information
//the first entry in the vector is BookID
//the second entry in the vector is ISBN10
//the third entry in the vector is ISBN13
//the fourth entry in the vector is Title
//the fifth entry in the vector is Edition
//the sixth entry in the vector is Author
//if the book does not already exist in the database then an empty vector is returned


//the purpose of this function is to create a new book listing
//the function requires a database point, username, isbn10, isbn13, title, edition, author, 
//course_code, price, and condition of the book as strings.  Also a bool indicating if it has 
//been written in and a string of commments
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
//the function returns void and makes the necesarry updates to the database

//helper function for book searching functions
vector<string> get_books_for_sale(Database* db, string bookid){
	vector<map<string, string>> results;
	string q = "select Merchant, Price, Condition, Comments, SaleID from Sale where Book = " + bookid 
		+  "AND Sold = 0";
	results = db->query(q);
	vector<string> return_results;
	if(!results.size()) return return_results;
	vector<map<string, string>> results2;
	for(int i=0; i<results.size(); i++){
		return_results.push_back(results[i]["Price"]);
		return_results.push_back(results[i]["Condition"]);
		return_results.push_back(results[i]["Comments"]);
		string sellerid = results[i]["Merchant"];
		q = "select Fname, Lname from Users where UserID = " + sellerid + ";";
		results2 = db->query(q);
		return_results.push_back(results2[0]["Fname"]);
		return_results.push_back(results2[0]["Lname"]);
		results2.clear();
	}
	return return_results;
}



//takes a pointer to the database object and an isbn10 as a string
vector<string> find_by_isbn10(Database* db, string isbn10){
	string q = "select BookID from Books where ISBN10 = '" + isbn10 + "';";
	vector<map<string, string>> result = db->query(q);
	vector<string> return_result;
	string bookid = result[0]["BookID"];
	return_result = get_books_for_sale(db, bookid);
	result.clear();
	q = "select ISBN10, Title, Edition from Books where BookID = " + bookid + ";";
	result = db->query(q);
	return_result.push_back(result[0]["Title"]);
	return_result.push_back(result[0]["Edition"]);
	return_result.push_back(result[0]["ISBN10"]);
	return return_result;
}
//returns a vector of strings.
//The first entry is the price of the listed book
//the second entry is the condition of the listed book
//the third entry is comments associated with the listing
//the fourth entry is the first name of the seller
//the fifth entry is the last name of the seller
//the sixth entr is the title of the book
//the seventh entry is the edition of the book
//the eight entry is the ISBN10 of the book

//takes a pointer to the database object and an isbn13 as a string
vector<string> find_by_isbn13(Database* db, string isbn13){
	string q = "select BookID from Books where ISBN13 = '" + isbn13 + "';";
	vector<map<string, string>> result = db->query(q);
	vector<string> return_result;
	string bookid = result[0]["BookID"];
	return_result = get_books_for_sale(db, bookid);
	result.clear();
	q = "select ISBN10, Title, Edition from Books where BookID = " + bookid + ";";
	result = db->query(q);
	return_result.push_back(result[0]["Title"]);
	return_result.push_back(result[0]["Edition"]);
	return_result.push_back(result[0]["ISBN10"]);
	return return_result;
}
//returns a vector of strings.
//The first entry is the price of the listed book
//the second entry is the condition of the listed book
//the third entry is comments associated with the listing
//the fourth entry is the first name of the seller
//the fifth entry is the last name of the seller
//the sixth entr is the title of the book
//the seventh entry is the edition of the book
//the eight entry is the ISBN10 of the book



//takes a pointer to the database object and a title as a string
vector<string> find_by_title(Database* db, string title){
	string q = "select BookID from Books where Title = '" + title + "';";
	vector<map<string, string>> result = db->query(q);
	vector<string> return_result;
	string bookid = result[0]["BookID"];
	return_result = get_books_for_sale(db, bookid);
	result.clear();
	q = "select ISBN10, Title, Edition from Books where BookID = " + bookid + ";";
	result = db->query(q);
	return_result.push_back(result[0]["Title"]);
	return_result.push_back(result[0]["Edition"]);
	return_result.push_back(result[0]["ISBN10"]);
	return return_result;
}
//returns a vector of strings.
//The first entry is the price of the listed book
//the second entry is the condition of the listed book
//the third entry is comments associated with the listing
//the fourth entry is the first name of the seller
//the fifth entry is the last name of the seller
//the sixth entr is the title of the book
//the seventh entry is the edition of the book
//the eight entry is the ISBN10 of the book



//takes a pointer to the database object and an author as a string
vector<string> find_by_author(Database* db, string author){
	vector<map<string, string>> result;
	string q = "select Book from Authors where Author = '" + author + "';";
	result = db->query(q);
	string bookid = result[0]["Book"];
	result.clear();
	vector<string> return_result;
	return_result = get_books_for_sale(db, bookid);
	result.clear();
	q = "select ISBN10, Title, Edition from Books where BookID = " + bookid + ";";
	result = db->query(q);
	return_result.push_back(result[0]["Title"]);
	return_result.push_back(result[0]["Edition"]);
	return_result.push_back(result[0]["ISBN10"]);
	return return_result;
}
//returns a vector of strings.
//The first entry is the price of the listed book
//the second entry is the condition of the listed book
//the third entry is comments associated with the listing
//the fourth entry is the first name of the seller
//the fifth entry is the last name of the seller
//the sixth entr is the title of the book
//the seventh entry is the edition of the book
//the eight entry is the ISBN10 of the book

//This function does not email the user but would if this program were fully implemented
//this function performs the act of selling a book
//the function takes a database pointer, isbn10, sellers first name, sellers last name, 
//and the username of the buyer as strins
void contact_seller(Database* db, string isbn10, string seller_f_name, string seller_l_name, string u_name){
	vector<map<string, string>> result;
	string q = "select BookID from Books where ISBN10 = '" + isbn10 + "';";
	result = db->query(q);
	string bookid = result[0]["BookID"];
	result.clear();
	q = "select UserID from Users where FName = '" + seller_f_name + "' AND LName = '" + 
		seller_l_name + "';";
	result = db->query(q);
	string merchantid = result[0]["UserID"];
	result.clear();
	q = "select UserID from Users where UName = '" + u_name + "';";
	result = db->query(q);
	string buyerid = result[0]["UserID"];
	result.clear();
	q = "select count(PurchaseID) from Purchases";
	result = db->query(q);
	int purchase = atoi(result[0].begin()->second.c_str()) + 1;
	char *c1 = new char;
	c1 = itoa(purchase, c1, 10);
	string purchaseid = c1;
	result.clear();
	q = "select SaleID from Sale where Book = " + bookid + " AND Merchant = " + merchantid + ";";
	result = db->query(q);
	string saleid = result[0]["SaleID"];
	q = "update Sale SET Sold=1 where Book = " + bookid + " AND Merchant = " + merchantid + ";";
	db->query(q);
	q = "INSERT INTO Purchases VALUES(";
	q = q + purchaseid + ", ";
	q = q + buyerid + ", ";
	q = q + merchantid + ", ";
	q = q + saleid + ", ";
	q = q + "CURRENT_TIMESTAMP);";
	db->query(q);
}
//the function returns void when the sale is complete

//returns a vector of strings indicating the titles of all books a user is selling
//takes the users username as a string and a database pointer
vector<string> view_user_listings(Database* db, string u_name){
	string q = "select UserID from Users where UName = '" + u_name + "';";
	vector<map<string, string>> result = db->query(q);
	string id = result[0]["UserID"];
	result.clear();
	q = "select Title from Books where BookID IN (select Book from Sale where Merchant = " + id + ")";
	result = db->query(q);
	vector<string> return_results;
	for(int i=0; i<result.size(); i++){
		return_results.push_back(result[i].begin()->second);
	}
	return return_results;
}
//ends with the returning of all the titles of books the user is selling as a vector of strings

//End of David's section