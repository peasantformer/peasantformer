#include "OpenGL.h"


void PeasantGenericTestModule::test() {
	printf("working\n");
}

void PeasantGenericTestModule::render() {
}

void PeasantGenericTestModule::redraw() {
}

void PeasantGenericTestModule::flip() {
	printf("flipping\n");
}

extern "C" PeasantGenericModuleInfo info() {
	return PeasantGenericModuleInfo(PM_RENDER,"opengl","OpenGL renderer module","1.0","Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" PeasantRenderModuleInfo render_info() {
	return PeasantRenderModuleInfo(PRM_HARDWARE);
}

extern "C" PeasantGenericTestModule *construct() {
	return new PeasantGenericTestModule;
}

extern "C" void deconstruct(PeasantGenericTestModule *m) {
	delete m;
}

