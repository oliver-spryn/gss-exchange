#include "BaseException.h"

class DatabaseConnectionException : public BaseException {
public:
	DatabaseConnectionException(string error);
};