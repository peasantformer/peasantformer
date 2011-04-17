#ifndef PEASANTFORMER_Data_World
#define PEASANTFORMER_Data_World

#include <Data/Generic/GenericTypes.h>
#include <Data/Section/Section.h>
#include <Data/Array/Array.h>

class World;

class WorldData {
	friend class World;
	private:
		Array <Particle> particles;
		Array <Section> sections;	
};

class World {
	
	private:
		WorldData *data;
		std::list<Particle *> particles;
		std::list<Section *> sections;
		
		PeasantSize s_w;
		PeasantSize s_h;
		
		Section *u;
		Section *d;
		Section *l;
		Section *r;
	public:
		World();
		World(PeasantSize s_w, PeasantSize s_h);
	
	public:
		Section *add_section(Section sc);
		Particle *add_particle(Particle pt);
	public:
		std::list<Section *>::iterator get_sections_begin();
		std::list<Section *>::iterator get_sections_end();
		std::list<Particle *>::iterator get_particles_begin();
		std::list<Particle *>::iterator get_particles_end();
	public:
	
		bool move_particles(bool init=false);
		
		Section *gen_new_section(Section *current_section, Direction direction, bool gen = false);
		
	public:
		void set_u(Section * value);
		void set_d(Section * value);
		void set_l(Section * value);
		void set_r(Section * value);

	public:
		Section * get_u(void);
		Section * get_d(void);
		Section * get_l(void);
		Section * get_r(void);
};

#endif
