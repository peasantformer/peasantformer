#ifndef PEASANTFORMER_RENDER_MODULE_OPENGL
#define PEASANTFORMER_RENDER_MODULE_OPENGL


class PeasantGenericTestModule : public PeasantRenderModuleInterface {
	public:
		void test();
		void render();
		void redraw();
		void flip();
};

#endif
