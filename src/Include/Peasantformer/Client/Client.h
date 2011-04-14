#ifndef PEASANTFORMER_Client
#define PEASANTFORMER_Client

#include <Peasantformer/Client/Network.h>
#include <Peasantformer/Client/IO.h>
#include <Peasantformer/Client/Render.h>
#include <Peasantformer/Modules/Modules.h>
#include <Peasantformer/Modules/Interfaces/RenderInterface.h>
#include <Peasantformer/Modules/Interfaces/ObjectInterface.h>


#include <stdio.h>

class ClientEngine {
	public:
		ClientNetwork network;
		Modules *modules;
		RenderInterface *render;
	public:
		ClientEngine() {
			this->modules = new Modules(
				 true // render
				,true // object
			);
		}
		~ClientEngine() {
			delete this->modules;	
		}
};

#endif
