#ifndef PEASANTFORMER_Module_Render_OpenGL
#define PEASANTFORMER_Module_Render_OpenGL

#include <Peasantformer/Modules/Interfaces/RenderInterface.h>

class RenderOpenGL : public RenderInterface {
	public:
		void render();
		void clear();
		void flip();
};


#endif
