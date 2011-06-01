#ifndef PEASANTFORMER_Apps_Server
#define PEASANTFORMER_Apps_Server

/// @file

class ServerEngine;

#include <signal.h>

#include <Apps/Server/ServerNetwork/ServerNetwork.h>
#include <Apps/Server/ServerConnections/ServerConnections.h>

#include <Apps/Server/ThreadConnection/ThreadConnection.h>
#include <Apps/Server/ThreadLogin/ThreadLogin.h>
#include <Apps/Server/ThreadAdminInput/ThreadAdminInput.h>


#include <Proto/Network/Network.h>
#include <Proto/PlainIO/PlainIO.h>
#include <Data/PString/PString.h>

/// Server engine class
class ServerEngine {
	private: 
		size_t buffsize; ///< size of all buffers used for cliet-servr
		                 ///< communications; in bytes
	public:
		/// Gets buffers size
		///
		/// @return buffer size
		size_t get_buffsize();
	public:
		/// Constructor
		ServerEngine();
		/// Destructor
		~ServerEngine();
	public:
		ServerNetwork *network;         ///< networking class
		ServerConnections *connections; ///< connections class
		
		ThreadConnection *thread_connection;  ///< connection handler thread
		ThreadLogin      *thread_login;       ///< login handler thread
		ThreadAdminInput *thread_admin_input; ///< admin input handler thread
};


#endif
