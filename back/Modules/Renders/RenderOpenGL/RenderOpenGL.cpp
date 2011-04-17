#include "RenderOpenGL.h"

void RenderOpenGL::render() {
}
void RenderOpenGL::clear() {
}
void RenderOpenGL::flip() {
}


extern "C" ModuleInfo info() {
	return ModuleInfo(MODULE_RENDER,"opengl","OpenGL renderer module","1.0","Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" RenderInfo render_info() {
	return RenderInfo(RENDER_HARDWARE);
}

extern "C" RenderOpenGL *construct() {
	return new RenderOpenGL;
}

extern "C" void destruct(RenderOpenGL *m) {
	delete m;
}
