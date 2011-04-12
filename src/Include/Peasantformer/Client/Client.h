#ifndef PEASANTFORMER_Client
#define PEASANTFORMER_Client

#include <Peasantformer/Client/Network.h>
#include <Peasantformer/Client/IO.h>
#include <Peasantformer/Client/Render.h>
#include <Peasantformer/Modules/Modules.h>
#include <Peasantformer/Modules/Interfaces/RenderInterface.h>
#include <Peasantformer/Modules/Interfaces/PeasantObjectInterface.h>


class ClientEngine {
	public:
		ClientNetwork network;
		Modules< RenderModule<render_construct,render_destruct> > render_modules;
	public:
		ClientEngine() {
		}
};

#endif
