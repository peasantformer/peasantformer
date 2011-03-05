#include <stdio.h>
#include <cmath>
#include <vector>
#include "SDL/SDL.h"

SDL_Surface *screen;

class Vector2;
Vector2 operator*(Vector2, float);

class Vector2 {
	public:
		float x,y;
	public:
		Vector2() {
			this->x = 0.0f;
			this->y = 0.0f;
		}
		Vector2(float x, float y) {
			this->x = x;
			this->y = y;
		}
	public:
		inline const Vector2 operator+=(Vector2 r) {
			this->x += r.x;
			this->y += r.y;
			return (*this);

		}
		inline const Vector2 operator-=(Vector2 r) {
			this->x -= r.x;
			this->y -= r.y;
			return (*this);

		}
		inline const Vector2 operator*=(Vector2 r) {
			this->x *= r.x;
			this->y *= r.y;
			return (*this);

		}
	public:
		inline const float square_length() {
			return (x*x + y*y);
		}
		inline const float length() {
			return sqrt(this->square_length());
		}
		inline const Vector2 normalize() {
			float len = this->length();
			if (len == 0)
				return Vector2(0,0);
			float inv_length = 1.0f / len;
			return ((*this)	 * inv_length);	
		}
};

inline Vector2 operator*(Vector2 l, float r) {
	return Vector2(l.x*r, l.y*r);
}

inline Vector2 operator/(Vector2 l, float r) {
	return Vector2(l.x/r, l.y/r);
}

inline Vector2 operator*(Vector2 l, Vector2 r) {
	return Vector2(l.x*r.x, l.y*r.y);
}

inline Vector2 operator/(Vector2 l, Vector2 r) {
	return Vector2(l.x/r.x, l.y/r.y);
}

inline Vector2 operator+(Vector2 l, Vector2 r) {
	return Vector2(l.x+r.x, l.y+r.y);
}

inline Vector2 operator-(Vector2 l, Vector2 r) {
	return Vector2(l.x-r.x, l.y-r.y);
}

template <class T>
class Array {
	private:
		size_t count;
		size_t alloc;
		std::vector<T> data;
	public:
		Array() {
			this->count = 0;
			this->alloc = 0;
		}
		~Array() {
			this->count = 0;
			this->alloc = 0;
			this->data.clear();
		}
	public:
		T & operator[](size_t i) {
			return this->data[i];
		}
		Array & operator=(Array const &r) {
			this->data.clear();
			for (size_t i=0; i < r.size(); i++) {
				this->data.push_back(r.data[i]);
			}
			return *this;
		}
	public:
		size_t add_item(T item) {
			this->count++;
			if (this->count > this->alloc) {
				this->alloc++;
				this->data.resize(this->alloc);
			}
			this->data[this->count-1] = item;
			return this->count-1;
		}
		void del_item (size_t i) {
			if (this->count == 0) return;
			this->count--;
			if (this->count == i) return;
			this->data[i] = this->data[this->count];
		}
		size_t size() const {
			return this->count;
		}
		void clear() {
			this->count = 0;
			this->alloc = 0;
			this->data.clear();
		}
};

class ReverseLink {
	friend class World;
	private:
		size_t global_id;
		size_t local_id;
	public:
		ReverseLink() {
			this->global_id = 0;
			this->local_id = 0;
		}
		ReverseLink(size_t global_id, size_t local_id) {
			this->global_id = global_id;
			this->local_id = local_id;
		}
};

class Particle {
	friend class World;
	friend class PhysComputorS;
	private:
		Array<size_t> collide_lids;
	
		size_t world_id;
		size_t level_id;
		Array<ReverseLink> rev_sections;
		

		Vector2 position;
		Vector2 speed;
		Vector2 projected_position;
		Uint32 color;
		float width, height;
		float inv_mass;
		float bounceness;
		bool is_pinned;
	public:
		Particle() {
			this->level_id = 0;
			this->world_id = 0;
			
			this->position = Vector2(0,0);
			this->speed = Vector2(0,0);
			this->projected_position = Vector2(0,0);
			this->color = SDL_MapRGB(screen->format,0xFF,0xFF,0xFF);
			this->width = 0;
			this->height = 0;
			this->inv_mass = 0;
			this->bounceness = 0;
			this->is_pinned = false;
		}
		Particle(Vector2 position, Vector2 speed, Uint32 color, float width, float height, float inv_mass, float bounceness, bool is_pinned) {
			this->level_id = 0;
			this->world_id = 0;
			
			this->position = position;
			this->speed = speed;
			this->projected_position = Vector2(0,0);
			this->color = color;
			this->width = width;
			this->height = height;
			this->inv_mass = inv_mass;
			this->bounceness = bounceness;
			this->is_pinned = is_pinned;
		}
	public:
		size_t add_collide_lid(size_t i) {
			return this->collide_lids.add_item(i);
		}
		size_t add_rev_section(ReverseLink rsec) {
			return this->rev_sections.add_item(rsec);
		}
	public:
		size_t get_collide_lid_size() {
			return this->collide_lids.size();
		}
		size_t get_rev_section_size() {
			return this->rev_sections.size();
		}
	public:
		size_t get_collide_lid(size_t i) {
			return this->collide_lids[i];
		}
		ReverseLink get_rev_section(size_t i) {
			return this->rev_sections[i];
		}
	public:
		void del_collide_lid(size_t i) {
			return this->collide_lids.del_item(i);
		}
		void del_rev_section(size_t i) {
			return this->rev_sections.del_item(i);
		}
	public:
		void clear_rev_section() {
			this->rev_sections.clear();
		}
	public:
		void draw() {
			SDL_Rect rect;
			rect.x = this->position.x - this->width/2;
			rect.y = this->position.y - this->height/2;
			rect.w = this->width;
			rect.h = this->height;
			SDL_FillRect(screen,&rect,color);
		}
};

class Section {
	friend class World;
	private:
		Array<size_t> particles;
		size_t world_id;
		size_t level_id;
		size_t x,y;
	public:
		Section() {
			this->x = 0;
			this->y = 0;
		}
		Section(size_t x, size_t y) {
			this->x = x;
			this->y = y;
		}
	public:
		size_t add_particle(size_t pti) {
			return this->particles.add_item(pti);
		}
	public:
		size_t get_particle_size() {
			return this->particles.size();
		}
	public:
		size_t get_particle(size_t i) {
			return this->particles[i];
		}
	public:
		void del_particle(size_t i) {
			this->particles.del_item(i);
		}
};
class Level {
	friend class World;
	private:
		Array<size_t> sections;
		Array<size_t> particles;
		
		size_t world_id;
		
		size_t x,y;
		size_t width,height;
		size_t section_width, section_height;
		
	public:
		Level() {
			this->world_id = 0;
		
			this->x = 0;
			this->y = 0;
			this->width = 0;
			this->height = 0;
			this->section_width = 0;
			this->section_height = 0;
		}
		Level(size_t x, size_t y, size_t width, size_t height, size_t section_width, size_t section_height) {
			this->world_id = 0;
			
			this->x = x;
			this->y = y;
			this->width = width/section_width;
			this->height = height/section_height;
			if (this->width == 0) this->width = 1;
			if (this->height == 0) this->height = 1;
			this->section_width = section_width;
			this->section_height = section_height;
		}
	public:
		size_t add_particle(size_t pt) {
			return this->particles.add_item(pt);
		}
		size_t add_section(size_t sc) {
			return this->sections.add_item(sc);
		}
	public:
		size_t get_particle_size() {
			return this->particles.size();
		}
		size_t get_section_size() {
			return this->sections.size();
		}
	public:
		size_t get_particle(size_t i) {
			return this->particles[i];
		}
		size_t get_section(size_t i) {
			return this->sections[i];
		}
	public:
		void del_particle(size_t i) {
			this->particles.del_item(i);
		}
		void del_section(size_t i) {
			this->sections.del_item(i);
		}
};

class World {
	friend class Universe;
	private:
		Array<Particle> particles;
		Array<Section> sections;
		Array<Level> levels;
		
		size_t universe_id;
	public:
		World() {
			this->universe_id = 0;
		}
	public:
		void del_particle_rev(size_t i) {
			Particle pt = this->particles[i];
			for (size_t n=0; n < pt.get_rev_section_size(); n++) {
				ReverseLink rev = pt.get_rev_section(n);
				this->sections[rev.global_id].del_particle(rev.local_id);
			}
		}
		void place_particle(size_t level_id, size_t id) {
			Level lvl = this->levels[level_id];
			Particle pt = this->particles[id];
			if (pt.position.x < 0 || pt.position.y < 0) return;
			
			size_t max_x = (pt.position.x + pt.width/2) / lvl.section_width;
			size_t min_x = (pt.position.x - pt.width/2) / lvl.section_width;
			size_t max_y = (pt.position.y + pt.height/2) / lvl.section_height;
			size_t min_y = (pt.position.y - pt.height/2) / lvl.section_height;

		
			size_t sec_wid;
			size_t par_sid;

			for (size_t i=min_x; i <= max_x; i++) {
				if (i >= lvl.width || i < 0) continue;
				for (size_t n=min_y; n <= max_y; n++) {
					if (n >= lvl.width || n < 0) continue;
					sec_wid = lvl.height * i + n;
					par_sid = this->sections[lvl.get_section(sec_wid)].add_particle(id);
					this->particles[id].add_rev_section(ReverseLink(sec_wid,par_sid));
				}
			}
		}

		void rebuild_links(size_t level_id) {
			size_t id;
			for (size_t n=0; n < this->levels[level_id].get_particle_size(); n++) {
				id = this->levels[level_id].get_particle(n);
				this->del_particle_rev(id);
				this->place_particle(level_id,id);			
			}

		}
	public:
		size_t add_level(Level level) {
			size_t wid;
			wid = this->levels.add_item(level);
			this->levels[wid].world_id = wid;

			
			size_t sid;
			for (size_t x=0; x < level.width; x++) {
				for (size_t y=0; y < level.height; y++) {
					sid = this->sections.add_item(Section(x,y));
					this->levels[wid].add_section(sid);
				}
			}
			return wid;
		}
		size_t add_particle(size_t level_id, Particle pt) {
			size_t lid, wid;
			wid = this->particles.add_item(pt);
			lid = this->levels[level_id].add_particle(wid);
			this->particles[wid].world_id = wid;
			this->particles[wid].level_id = lid;
			return wid;
		}
		size_t add_section(size_t level_id, Section sc) {
			size_t lid, wid;
			wid = this->sections.add_item(sc);
			lid = this->levels[level_id].add_section(wid);
			this->sections[wid].world_id = wid;
			this->sections[wid].level_id = lid;
			return wid;
		}
	public:
		size_t get_level_size() {
			return this->levels.size();
		}
		size_t get_particle_size() {
			return this->particles.size();
		}
		size_t get_section_size() {
			return this->sections.size();
		}
	public:
		Level* get_level(size_t i) {
			return &this->levels[i];
		}
		Particle* get_particle(size_t i) {
			return &this->particles[i];
		}
		Section* get_section(size_t i) {
			return &this->sections[i];
		}
	public:
		void del_level(size_t i) {
			Level lvl = this->levels[i];
			for (size_t n = 0; n< lvl.get_particle_size(); n++) {
				this->del_particle(lvl.get_particle(n));
			}
			for (size_t n = 0; n< lvl.get_section_size(); n++) {
				this->del_section(lvl.get_section(n));
			}
			this->levels.del_item(i);
		}
                // this function never should be used outisde of del_level(), 
                // yet  it doesn't worth putting it into private state :P
		void del_section(size_t i) { 
			this->sections.del_item(i);
		}
		void del_particle(size_t i) {
//			this->del_particle_rev(i);
			this->particles.del_item(i);
		}
};

class Universe {
	private:
		Array <World> worlds;
	public:
		Universe() {
		}
	public:
		size_t add_world(World world) {
			size_t universe_id;
			universe_id = this->worlds.add_item(world);
			this->worlds[universe_id].universe_id = universe_id;
			return universe_id;
		}
	public:
		size_t get_world_size() {
			return this->worlds.size();
		}
	public:
		World *get_world(size_t i) {
			return &this->worlds[i];
		}
	public:
		void del_world(size_t i) {
			this->worlds.del_item(i);
		}
	
};


class PhysComputorS {
	private:
		World *world;
		Vector2 gravity;
		float dt;
	public:
		PhysComputorS() {
			this->world = NULL;
			this->gravity = Vector2(0,0);
			this->dt = 0.01;
		}
		PhysComputorS(World *world) {
			this->world = world;
			this->gravity = Vector2(0,9.8);
			this->dt = 0.01;
		}
	public:
		void iterate() {
			for (size_t i=0; i < world->get_particle_size(); i++) {
				if (world->get_particle(i)->is_pinned == true) continue;
				world->get_particle(i)->speed += gravity * dt;
				world->get_particle(i)->projected_position = world->get_particle(i)->position + world->get_particle(i)->speed * dt;
			}


			for (size_t z=0; z < world->get_section_size(); z++) {
				Section *sec = world->get_section(z);
				for (size_t i=0; i < sec->get_particle_size(); i++) {
					for (size_t n=i+1; n < sec->get_particle_size(); n++) {
					}
				}
			}

			for (size_t i=0; i < world->get_particle_size(); i++) {
				if (world->get_particle(i)->is_pinned == true) continue;
				world->get_particle(i)->speed = (world->get_particle(i)->projected_position - world->get_particle(i)->position) / dt;
				world->get_particle(i)->position  = world->get_particle(i)->projected_position;
			}
		}
};
int main(int argc, char **argv) {
	size_t world_id;
	size_t level_id;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1100,600,32,SDL_SWSURFACE);

	Universe universe;
	world_id = universe.add_world(World());
	World *world = universe.get_world(world_id);

	level_id = world->add_level(Level(0, 0, 500, 500, 20, 20));
	
	world->add_particle(level_id,Particle(Vector2(10,10), Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0xFF,0xFF), 5, 5, 1, 0, false));
	world->add_particle(level_id,Particle(Vector2(20,10), Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0xFF,0xFF), 5, 5, 1, 0, false));
	world->add_particle(level_id,Particle(Vector2(30,10), Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0xFF,0xFF), 5, 5, 1, 0, false));
//	world->add_particle(level_id,Particle(Vector2(20,10), Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0xFF,0xFF), 5, 5, 1, 0, false));
//	for (size_t i=0; i < 10; i++) {
//		world->add_particle(level_id,Particle(Vector2(10+i*10,10), Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0xFF,0xFF), 5, 5, 1, 0, false));
//	}
	Level *level = world->get_level(level_id);

	PhysComputorS computor(world);

//	world->rebuild_links(level_id);

//	printf("%d\n",(int)world->get_section_size());

	bool quit = false; 
	SDL_Event event;
	
	while  (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) quit = true;
		
		int ticks = SDL_GetTicks();

		for (size_t i=0; i < world->get_level_size(); i++) {
			world->rebuild_links(i);
		}
		
		computor.iterate();
	
		printf("%d @ %d\n",SDL_GetTicks()-ticks,(int)level->get_particle_size());
	
		SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));

		for (size_t i=0; i < level->get_particle_size(); i++) {
			world->get_particle(i)->draw();
		}
		
		SDL_Flip(screen);
	}
	return 0;	
}
