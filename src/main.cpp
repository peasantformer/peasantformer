#include "main.h"


int main (int argc, char **argv) {
	PeasantCore *core;
	core = new PeasantCore("modules");
	
	core->scan_modules_dir();
	core->load_modules();
	
	core->get_render_modules()->print();
	PeasantRenderModule m = core->get_render_modules()->get_render_module_by_name("opengl");
	printf("%s\n",m.get_name().c_str());
	
	delete core;
	return 0;
}
