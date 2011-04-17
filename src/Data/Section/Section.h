#ifndef PEASANTFORMER_Data_Section
#define PEASANTFORMER_Data_Section

#include <list>

#include <Data/Particle/Particle.h>
#include <Data/Generic/GenericTypes.h>

class Particle;

class Section {
	private:
		PeasantID id;
		
		PeasantPos x;
		PeasantPos y;
		PeasantSize w;
		PeasantSize h;
		
		Section *u;
		Section *d;
		Section *l;
		Section *r;
		
		std::list<Particle *> particles;
	public:
		Section();
		Section(PeasantPos x
		       ,PeasantPos y
		       ,PeasantSize w = 0
		       ,PeasantSize h = 0);
	public:
		void set_id(PeasantID value);
		void set_x(PeasantPos value);
		void set_y(PeasantPos value);
		void set_w(PeasantSize value);
		void set_h(PeasantSize value);
		void set_u(Section * value);
		void set_d(Section * value);
		void set_l(Section * value);
		void set_r(Section * value);
	public:
		PeasantID get_id(void);
		PeasantPos get_x(void);
		PeasantPos get_y(void);
		PeasantSize get_w(void);
		PeasantSize get_h(void);
		Section * get_u(void);
		Section * get_d(void);
		Section * get_l(void);
		Section * get_r(void);
};

#endif
