#include "Engine.h"

void PeasantEngineModule::test() {
	printf("working\n");
}

extern "C" ModuleInfo info() {
	ModuleInfo minfo;
	minfo.name = (char*)"engine";
	minfo.descr = (char*)"Peasantformer Engine module";
	minfo.version = (char*)"1.0";
	return minfo;
}

extern "C" PeasantEngineModule *create() {
	return new PeasantEngineModule;
}

extern "C" void destroy(PeasantEngineModule *m) {
	delete m;
}
