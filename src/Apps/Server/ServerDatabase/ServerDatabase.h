#ifndef PEASANTFORMER_Apps_ServerDatabase
#define PEASANTFORMER_Apps_ServerDatabase

/// @file

class ServerDatabase;

#include <Apps/Server/Server.h>
#include <Proto/Database/Database.h>
#include <Data/Hash/Hash.h>
#include <Data/PString/PString.h>

/// Databae interaction class
class ServerDatabase {
	private:
		ServerEngine *engine; ///< Server class instance
		DatabaseInterface *db_users; ///< database of users
	public:

		/// Constructor
		///
		/// \param [in] srvr is Server instance
		ServerDatabase(ServerEngine *srvr);
		
		/// Destructor
		~ServerDatabase();
	public:
	
		/// Lookes up user with given login and password hash
		///
		/// \param [in] login is username
		/// \param [in] password_hash is password_hash
		/// @return Hash with results
		std::vector< Hash<PString> > db_users_lookup_user(PString login, PString password_hash);
};

#endif
