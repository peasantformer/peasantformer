#ifndef PEASANTFORMER_Apps_Server_Database
#define PEASANTFORMER_Apps_Server_Database

class ServerDatabase;

#include <Apps/Server/Server.h>
#include <Proto/PlainIO/PlainIOHighLevel.h>
#include <Proto/Database/Database.h>

class ServerDatabase {
	private:
		Server *engine;
		Database *db_users;
	public:
		ServerDatabase(Server *srvr, pio_string db_prefix);
		~ServerDatabase();
	public:
		int user_lookup_by_password_username(
			const pio_string &i_password_hash, 
			const pio_string &i_username,
			pio_string &o_password_hash,
			pio_string &o_username,
			pio_string &o_nickname,
			pio_string &o_email
		);
};

#endif
