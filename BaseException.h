#ifndef BASEEXCEPTION_H
#define BASEEXCEPTION_H

#include <exception>
#include <string>
#include <ostream>

using std::exception;
using std::ostream;
using std::string;

class BaseException : public exception {
	friend ostream &operator<< (ostream &out, BaseException be);

public : 
	BaseException(string error);

private : 
	string error;
};

#endif