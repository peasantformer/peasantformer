#include "Square.h"


extern "C" ModuleInfo info() {
	return ModuleInfo(MODULE_RENDER,"opengl","OpenGL renderer module","1.0","Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" PeasantObjectInfo render_info() {
	return PeasantObjectInfo(OBJECT_OBJECT);
}

extern "C" ObjectSquare *construct() {
	return new ObjectSquare;
}

extern "C" void destruct(ObjectSquare *m) {
	delete m;
}
