#ifndef PEASANTFORMER_Server
#define PEASANTFORMER_Server

#include <Peasantformer/Server/Network.h>
#include <Peasantformer/Modules/Modules.h>
#include <Peasantformer/Modules/Interfaces/ObjectInterface.h>


#include <stdio.h>

class ServerEngine {
	public:
		ServerNetwork network;
		Modules *modules;
	public:
		ServerEngine() {
			this->modules = new Modules(
				false // render
				,true // object
			);
		}
		~ServerEngine() {
			delete this->modules;
		}
};

#endif
