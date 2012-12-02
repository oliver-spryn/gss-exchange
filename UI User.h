
class User{
public:
	string get_new_User(string& new_Username);
	string get_new_Password(string& new_Password);
	void user_Login(string& user_Name, string& user_Password);

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
	cout << "That username is not valid. Please enter another: ";
		get_new_User(user_Name);
	}
}

