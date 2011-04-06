#ifndef PEASANTFORMER_RENDER_MODULE_OPENGL
#define PEASANTFORMER_RENDER_MODULE_OPENGL

#include <peasantformer/RenderInterface.h>
#include <stdio.h>

class PeasantGenericTestModule : public PeasantRenderModuleInterface {
	public:
		void test();
		void render();
		void redraw();
		void flip();
};

#endif
