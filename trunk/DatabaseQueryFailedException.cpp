#include "DatabaseQueryFailedException.h"

DatabaseQueryFailedException::DatabaseQueryFailedException(string error) : BaseException(error) {
	//This constructor simply calls the super constructor
}