#ifndef PEASANTFORMER_Database_Interface
#define PEASANTFORMER_Database_Interface

class DatabaseInterface {
	public:
		int connect();
		int prepare();
		int step();
		int column();
};

#endif
