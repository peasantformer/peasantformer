#include "Login.h"


int DataBaseLogin::lookup_user_by_password_username(
	const pio_string &username,
	const pio_string &password,
	pio_string &name,
	pio_string &nick,
	pio_string &hash
) {
	pio_string query("select * from users where username = :username AND passwordhash = :phash");
	this->db_users->prepare(query);
	this->db_users->bind_text(1,username);
	this->db_users->bind_text(2,password);
	this->db_users->step();
	if (!this->db_users->isRow()) return -1;
	hash.set(this->db_users->get_text("passwordhash"));
	name.set(this->db_users->get_text("username"));
	nick.set(this->db_users->get_text("nickname"));
	this->db_users->step();
	if (!this->db_users->isDone()) return -2;

	return 0;
}

