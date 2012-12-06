#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "DatabaseAttributeTypeUnknown.h"
#include "DatabaseConnectionException.h"
#include "DatabaseQueryFailedException.h"

class User{
public:
	string get_new_User(string& new_Username);
	string get_new_Password(string& new_Password);
	void user_Login(string& user_Name, string& user_Password);
	void post_Book();
	void browse_Books();
	void contact_Seller();
	void logout();
	void exit();
private:
};