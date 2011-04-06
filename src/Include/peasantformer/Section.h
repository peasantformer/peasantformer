#ifndef PEASANTFORMER_TYPE_SECTION
#define PEASANTFORMER_TYPE_SECTION

#include "Types.h"

class PeasantWorldSection {
	protected:
		PeasantID id;
		
		PeasantPos x;
		PeasantPos y;
		PeasantSize w;
		PeasantSize h;
		
		PeasantWorldSection *u;
		PeasantWorldSection *d;
		PeasantWorldSection *l;
		PeasantWorldSection *r;
		
		//std::list<Particle *> particles;
};

#endif
