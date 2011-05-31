#include "Server.h"

/// @file

ServerEngine::ServerEngine() {
	network = new ServerNetwork(this);
	//connections = new ServerConnections(this);
};

/// Main function.
///
/// \param [in] argc is a number of arguments
/// \param [in] argv is an argument list
/// @return always zero.
int main(int argc, char **argv) {
	pio_init();
	pn_init();
	//printf("%s\n",PString("яфы").c_str());
	//CircularBufffer cbuf(4);
	//cbuf.write("1");
	//cbuf.dump();
	//cbuf.disable_overflow();	
	//printf("%d\n",cbuf.read(5).length());
	//ConnectionPending pend;
	return 0;
}
