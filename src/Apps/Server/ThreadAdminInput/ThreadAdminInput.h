#ifndef PEASANTFORMER_Apps_Server_ThreadAdminInput
#define PEASANTFORMER_Apps_Server_ThreadAdminInput

/// @file

class ThreadAdminInput;

#include <pthread.h>
#include <unistd.h>

#include <Apps/Server/Server.h>
#include <Proto/Network/NetworkHighLevel.h>

/// Handles admin input from stdin
class ThreadAdminInput {
	private:
		ServerEngine *engine; ///< Instance of ServerEngine
		pthread_t thread_id;  ///< thread id
		bool time_to_exit;    ///< if time to stop connection thread has come
	private:
		
		/// Thread admin input handler worker
		///
		/// \param [in] data is instance of ThreadConnection class
		/// @return NULL
		static void *thread_admin_input_worker(void *data);
	public:
	
		/// Constructor
		///
		/// \param [in] srvr is ServerEngine instance
		ThreadAdminInput(ServerEngine *srvr);
		
		/// Destructor.
		~ThreadAdminInput();
	public:
	
		/// Sets up admin input thread
		pthread_t setup();
		
		/// Desetups admin input thread
		void desetup();
};

#endif
