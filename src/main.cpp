#include "main.h"

int main (int argc, char **argv) {
	PeasantEngine *engine;
	engine = new PeasantEngine();
	printf("%s\n",getenv("HOME"));
	/*
	PeasantCore *core;
	core = new PeasantCore("modules");
	
	core->scan_modules_dir();
	core->load_modules();
	
	printf("\n");
	core->get_render_modules()->print();
	printf("\n");
	core->get_object_modules()->print();

	//PeasantEngine *engine;
	//engine = new PeasantEngine();

		delete core;
	*/
	delete engine;
	return 0;
}
