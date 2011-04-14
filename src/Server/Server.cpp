#include <Peasantformer/Server/Server.h>

int main(int argc, char **argv) {
	ServerEngine engine;
	
	engine.modules->add_dir("modules");
	engine.modules->load_modules_in_dirs();
	
	printf("Object Modules:\n");
	for(std::map<std::string,ObjectModule>::iterator it=engine.modules->object_modules.begin(); it != engine.modules->object_modules.end(); it++) {
		ObjectModule mod = it->second;
		printf("  * %s - %s v%s\n",mod.get_name().c_str(),mod.get_description().c_str(),mod.get_version().c_str());
	}
	
	engine.network.setup_server_on_addr_port("","50600");
	
	
	
	return 0;
}
