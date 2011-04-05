#ifndef PEASANTFORMER_MODULE_TEST
#define PEASANTFORMER_MODULE_TEST

#include <stdio.h>
#include "../../Interfaces/RenderInterface.h"

class PeasantGenericTestModule : public PeasantRenderModuleInterface {
	public:
		void test();
		void render();
		void redraw();
		void flip();
};

#endif
