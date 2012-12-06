#include "UI Display.h";

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


string User::get_new_User(string& new_Username)
{
	cout << "Please enter your desired username and press enter: ";
	getline(cin, new_Username);		
	try{

		//Execute query to add a new username
	
	} catch (QueryFailedException e){
	cout << "That username is not valid. Please enter another: ";
		get_new_User(new_Username);
	}
}

string User::get_new_Password(string& new_Password)
{
	cout << "Please enter your password and press enter: ";
	getline(cin, new_Password);
}
	
void User::user_Login(string& user_Name, string& user_Password)
{
	cout << "Please enter your username: ";
	getline(cin, user_Name);
	cout << "Please enter your password: ";
	getline(cin, user_Password);
	try{

	//Execute query to add a new username

	} catch (QueryFailedException e){
	cout << "The username or password is not valid. Please enter another: ";
		user_Login(user_Name, user_Password);
	}
}

void User::browse_Books()
{
	int user_Choice;
	cout << "Please enter the number of the command you wish to execute:\n\n";
	cout << "\"[1]\" Browse by Author\n[2] Browse by Title\n\n";
	getline(cin, user_Choice);
	
	if(user_Choice == 1)
	{
		try{

		//Execute query to display books by Author

		} catch (QueryFailedException e){
		cout << "Author not found.";
		}
	}
	else if(user_Choice == 2)
	{
		try{

		//Execute query to display books by title

		} catch (QueryFailedException e){
		cout << "Title not found.";
		}
	}
}

void User::contact_Seller()
{

}

void User::exit()
{

}

