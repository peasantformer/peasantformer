#include "Login.h"


int DataBaseLogin::lookup_user_by_password_username(
	const pio_string &username,
	const pio_string &password,
	pio_string &name,
	pio_string &nick,
	pio_string &hash
) {
	pio_string query("select * from users where username = :username");
	this->db_users->prepare(query.c_str(),query.length());

}

