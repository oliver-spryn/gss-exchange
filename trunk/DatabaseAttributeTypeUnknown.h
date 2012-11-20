#include "BaseException.h"

class DatabaseAttributeTypeUnknown : public BaseException {
public:
	DatabaseAttributeTypeUnknown(string error);
};