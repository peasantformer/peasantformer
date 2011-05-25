#include "Database.h"

ServerDatabase::ServerDatabase(Server *srvr, pio_string db_prefix) {
	this->engine = srvr;
	this->db_users = new Database(db_prefix + "/users.db");
}

ServerDatabase::~ServerDatabase() {
	delete this->db_users;
}

int ServerDatabase::user_lookup_by_password_username(
	const pio_string &i_password_hash, 
	const pio_string &i_username,
	pio_string &o_password_hash,
	pio_string &o_username,
	pio_string &o_nickname,
	pio_string &o_email
) {
	pio_string query("select * from users where username = :username AND passwordhash = :phash");	
	this->db_users->prepare(query);
	this->db_users->bind_text(1,i_username);
	this->db_users->bind_text(2,i_password_hash);
	this->db_users->step();
	if (!this->db_users->isRow()) return -1;
	o_password_hash.set(this->db_users->get_text("passwordhash"));
	o_username.set(this->db_users->get_text("username"));
	o_nickname.set(this->db_users->get_text("nickname"));
	o_email.set(this->db_users->get_text("email"));
	this->db_users->step();
	if (!this->db_users->isDone()) return -2;
	this->db_users->finalize();
	return 0;
}

