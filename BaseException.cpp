#include <windows.h>

#include "BaseException.h"

using std::endl;

/**
 * CONSTRUCTOR 
 *
 * This constructor simply assigns the error string parameter to the member
 * variable "error" to be printed to the console later in a catch {} block.
 *
 * @access public
 * @param  string error A string containing a description of the problem
 * @return void
*/

BaseException::BaseException(string error) {
	this->error = error;
}

/**
 * This friend function overloads the output operator for this class for 
 * simple usage in a catch {} block after an exception has been thrown. 
 * It will display the provided error message in red text and sound an
 * alert beep upon output. This function allows simple usage of a custom
 * exception class, like this:
 *
 * ...
 * } catch(BaseException e) {
 *     cout << e;
 * }
 * ...
 *
 * @friend
 * @param  ostream&      out The stream which will output the message
 * @param  BaseException be  The exception to output
 * @return ostream&      out The same stream which was passed to this function
*/

ostream &operator<< (ostream &out, BaseException be) {
	HANDLE cHandle;
	cHandle = GetStdHandle(STD_OUTPUT_HANDLE);
	SetConsoleTextAttribute(cHandle, 12);

	out << "\a" << be.error << endl;

	SetConsoleTextAttribute(cHandle, 7);
	return out;
}