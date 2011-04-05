#include "main.h"


int main (int argc, char **argv) {
	PeasantCore *core;
	core = new PeasantCore("modules");
	
	core->scan_modules_dir();
	core->load_modules();
	
	printf("\n");
	core->get_render_modules()->print();
	printf("\n");
	core->get_object_modules()->print();

	PeasantEngine *engine;
	engine = new PeasantEngine();

	delete engine;
	delete core;
	return 0;
}
