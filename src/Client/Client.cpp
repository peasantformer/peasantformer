#include <Peasantformer/Client/Client.h>

int main(int argc, char **argv) {
	ClientEngine engine;
	
	engine.modules.add_dir("modules");
	engine.modules.load_modules_in_dirs();
	
	
	return 0;
}
