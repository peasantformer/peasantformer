#ifndef PEASANTFORMER_ENGINE_GENERIC_RENDER
#define PEASANTFORMER_ENGINE_GENERIC_RENDER

class PeasantRender {
	public:
		virtual ~PeasantRender() {}
	public:
		virtual void init() = 0;
		virtual void render() = 0;
		virtual void redraw() = 0;
		virtual void flip() = 0;
};

#endif
