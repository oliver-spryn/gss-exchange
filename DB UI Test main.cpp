#include <iostream>
#include <string>
using namespace std;

class User{
public:
	string get_new_User(string& new_Username);
	string get_new_Password(string& new_Password);
	void user_Login(string& user_Name, string& user_Password);

private:


};

class Admin : public User{
public:


private:


};

class Display{
public:
	void manage_Display();

private:


}

int main()
{
	Display current_Display;
	current_Display.manage_Display();
	return 0;
}
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

void Display::manage_Display()
{

	int user_Choice;
	cout << "Please enter the number of the command you wish to execute:\n\n";
	cout << "\"[1]\" Register\n[2] Login\n\n";
	cin >> user_Choice;
	while(user_Choice != 1 && user_Choice != 2)
	{
		cin.sync();
		cout << "Please try again:";
		cin >> user_Choice;
	}
	if(user_Choice = 1)
	{
		string new_Username;
		string new_Password;
		get_new_User(new_Username);
		get_new_Password(new_Password);
	
		char user_Continue = 'Y';
		while(user_Continue == 'Y')
		{
			int user_Command;
			cout << "Please enter the number of the command you wish to execute and press enter.\n\n";
			cout << "[1] Post a Book\n";
			cout << "[2] Browse Titles for Sale\n";
			cout << "[3] Contact a seller\n";
			cout << "[4] Logout\n";
			cout << "[5] Exit\n";
			cin >> user_Command;
			while(user_Command != 1 && user_Command != 2 && user_Command != 3 && user_Command != 4 && user_Command != 5)
			{
				cin.sync();
				cout << "Please try again:";
				cin >> user_Command;
			}
			switch(user_Command)
			{
			case 1:
		
			case 2:
		
			case 3:
		
			case 4:
		
			case 5:
		
			case 6:
		
			}
		}
	}
	else if(user_Choice = 2)
	{
		string user_Name;
		string user_Password;
		user_Login(user_Name, user_Password);
		while(true)
		{
			int user_Command;
			cout << "Please enter the number of the command you wish to execute and press enter.\n\n";
			cout << "[1] Post a Book\n";
			cout << "[2] Browse Titles for Sale\n";
			cout << "[3] Contact a seller\n";
			cout << "[4] Create Sellers\n";
			cout << "[5] Delete Users\n";
			cout << "[6] Browse Users\n";
			cout << "[7] Logout\n";
			cout << "[8] Exit\n";
			cin >> user_Command;
			while(user_Command != 1 && user_Command != 2 && user_Command != 3 && user_Command != 4 && user_Command != 5 && user_Command != 6)
			{
				cin.sync();
				cout << "Please try again:";
				cin >> user_Command;
			}
			switch(user_Command)
			{
			case 1:
				//Execute appropriate function 
			case 2:
				//Execute appropriate function
			case 3:
				//Execute appropriate function
			case 4:
				//Execute appropriate function
			case 5:
				//Execute appropriate function
			case 6:
				//Execute appropriate function
			}
			system("cls");
		}
	}
}
