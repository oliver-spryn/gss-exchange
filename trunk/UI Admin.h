#include <iostream>
#include <map>
#include <string>
#include <vector>
#include "UI User.h"
#include "DatabaseAttributeTypeUnknown.h"
#include "DatabaseConnectionException.h"
#include "DatabaseQueryFailedException.h"

class Admin : public User{
public:
	void create_Seller();
	void delete_Users();
	void browse_users();
private:

};