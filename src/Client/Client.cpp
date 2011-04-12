#include <Peasantformer/Client/Client.h>

int main(int argc, char **argv) {
	ClientEngine engine;
	engine.render_modules.add_dir("modules");
	engine.render_modules.load_modules_in_dirs();
	
	return 0;
}
