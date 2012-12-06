#include "BaseException.h"

class DistributeParametersInvalidException : public BaseException {
public:
	DistributeParametersInvalidException(string error);
};