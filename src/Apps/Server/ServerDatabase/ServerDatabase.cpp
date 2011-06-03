#include "ServerDatabase.h"

/// @file

ServerDatabase::ServerDatabase(ServerEngine *srvr) {
	engine = srvr;
	db_users = new DatabaseSqlite3("db/users.db");
}
ServerDatabase::~ServerDatabase() {
	delete db_users;
}

std::vector< Hash<PString> > ServerDatabase::db_users_lookup_user(PString login, PString password_hash) {
	std::vector< Hash<PString> > results;
	printf("%s %s\n",password_hash.c_str(),login.c_str());
	db_users->prepare("SELECT * from users where login = :login and password_hash = :phash");
	db_users->bind_string(1,login);
	db_users->bind_string(2,password_hash);
	printf(">> %s | %s\n",login.c_str(),password_hash.c_str());
	db_users->step();
	int i = 0;
	while (db_users->isRow()) {
		results.push_back(db_users->get_results());
		db_users->step();
		i++;
	}
	return results;
}
