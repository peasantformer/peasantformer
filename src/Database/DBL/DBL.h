#ifndef PEASANTFORMER_Database_DBL
#define PEASANTFORMER_Database_DBL

#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Database/DBL/Login/Login.h>

class DataBase {
	public:
		DataBaseLogin *login;
	public:
		DataBase(pio_string path) {
			this->login = new DataBaseLogin((path + "/users.db"));
		}
		~DataBase() {
			delete this->login;
		}

};

#endif
