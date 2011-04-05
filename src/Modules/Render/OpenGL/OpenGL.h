#ifndef PEASANTFORMER_RENDER_MODULE_OPENGL
#define PEASANTFORMER_RENDER_MODULE_OPENGL

#include <stdio.h>
#include "../../../Interfaces/RenderInterface.h"

class PeasantGenericTestModule : public PeasantRenderModuleInterface {
	public:
		void test();
		void render();
		void redraw();
		void flip();
};

#endif
