#include "BaseException.h"

class DatabaseQueryFailedException : public BaseException {
public:
	DatabaseQueryFailedException(string error);
};