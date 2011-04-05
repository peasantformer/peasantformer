#include "Test.h"


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

extern "C" PeasantModuleInfo info() {
	return PeasantModuleInfo(PM_TEST,"test","Test module","1.0");
}

extern "C" PeasantGenericTestModule *construct() {
	return new PeasantGenericTestModule;
}

extern "C" void deconstruct(PeasantGenericTestModule *m) {
	delete m;
}

