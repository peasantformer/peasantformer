#ifndef PEASANTFORMER_ENGINE
#define PEASANTFORMER_ENGINE

#include <stdio.h>
#include <SDL/SDL.h>
#include "Render/Generic.h"


class PeasantEngine {
	private:
		PeasantRender *render;
	public:
		
	public:
		void set_renderer(PeasantRender *);
};


#endif
