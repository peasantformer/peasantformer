#ifndef PEASANTFORMER_Apps_Server_ThreadLogin
#define PEASANTFORMER_Apps_Server_ThreadLogin

/// @file

class ThreadLogin;

#include <pthread.h>
#include <Apps/Server/Server.h>

/// Connection handler thread
class ThreadLogin {
	private:
		ServerEngine *engine; ///< Instance of ServerEngine
		pthread_t thread_id;  ///< thread id
		bool time_to_exit;    ///< if time to stop connection thread
	private:
		
		/// Thread connection worker
		///
		/// \param [in] data is instance of ThreadLogin class
		/// @return NULL
		static void *thread_login_worker(void *data);
	public:
	
		/// Constructor
		///
		/// \param [in] srvr is ServerEngine instance
		ThreadLogin(ServerEngine *srvr);
		
		/// Destructor.
		~ThreadLogin();
	public:
	
		/// Sets up connection thread
		pthread_t setup();
		
		/// Desetups connection thread
		void desetup();
};

#endif
