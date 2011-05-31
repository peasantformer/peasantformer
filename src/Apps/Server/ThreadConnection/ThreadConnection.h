#ifndef PEASANTFORMER_Apps_Server_ThreadConnection
#define PEASANTFORMER_Apps_Server_ThreadConnection

/// @file

class ThreadConnection;

#include <pthread.h>

#include <Apps/Server/Server.h>

/// Connection handler thread
class ThreadConnection {
	private:
		ServerEngine *engine; ///< Instance of ServerEngine
		pthread_t thread_id;  ///< thread id
		bool time_to_exit;    ///< if time to stop connection thread
	private:
		
		/// Thread connection worker
		///
		/// \param [in] data is instance of ThreadConnection class
		/// @return NULL
		static void *thread_connection_worker(void *data);
	public:
	
		/// Constructor
		///
		/// \param [in] srvr is ServerEngine instance
		ThreadConnection(ServerEngine *srvr);
		
		/// Destructor.
		~ThreadConnection();
	public:
	
		/// Sets up connection thread
		void setup();
		
		/// Desetups connection thread
		void desetup();
};

#endif
