#ifndef PEASANTFORMER_Database_DBL_Login
#define PEASANTFORMER_Database_DBL_Login

#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Database/DBInterface/DBInterface.h>

class DataBaseLogin {
	private:
		DatabaseInterface *db_users;
	public:
		DataBaseLogin(pio_string filename) {
			this->db_users = new DatabaseInterface(filename);
		}
		~DataBaseLogin() {
			delete this->db_users;
		}
	public:
		int lookup_user_by_password_username(
			const pio_string &username,
			const pio_string &password,
			pio_string &name, 
			pio_string &nick,
			pio_string &hash
		);
};

#endif
