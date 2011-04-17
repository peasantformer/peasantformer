#include <Peasantformer/Client/Client.h>

int main(int argc, char **argv) {
	ClientEngine engine;
	
	engine.modules->add_dir("modules");
	engine.modules->load_modules_in_dirs();
	
	printf("Render Modules:\n");
	for(std::map<std::string,RenderModule>::iterator it=engine.modules->render_modules.begin(); it != engine.modules->render_modules.end(); it++) {
		RenderModule mod = it->second;
			printf("  * %s - %s v%s\n",mod.get_name().c_str(),mod.get_description().c_str(),mod.get_version().c_str());
	}
	printf("Object Modules:\n");
	for(std::map<std::string,ObjectModule>::iterator it=engine.modules->object_modules.begin(); it != engine.modules->object_modules.end(); it++) {
		ObjectModule mod = it->second;
		printf("  * %s - %s v%s\n",mod.get_name().c_str(),mod.get_description().c_str(),mod.get_version().c_str());
	}
	
	return 0;
}
