#include "Square.h"


extern "C" ModuleInfo info() {
	return ModuleInfo(MODULE_OBJECT,"square","Sad emo square","1.0","Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" ObjectInfo object_info() {
	return ObjectInfo(OBJECT_RUNNER);
}

extern "C" ObjectSquare *construct() {
	return new ObjectSquare;
}

extern "C" void destruct(ObjectSquare *m) {
	delete m;
}

