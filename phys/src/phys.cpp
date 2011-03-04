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


struct RevSection {
	size_t s_wid;  // section world id
	size_t s_p_id; // section's particle id
};

class Particle {
	friend class World;
	private:
		Array<size_t> collide_wids;
		Array<RevSection> rev_sections;
	
		size_t p_lid;
		size_t p_wid;
		size_t level_id;

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
			this->p_lid = 0;
			this->p_wid = 0;
			
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
		Particle(Vector2 position, Vector2 speed, Vector2 projected_position, Uint32 color, float width, float height, float inv_mass, float bounceness, bool is_pinned) {
			this->p_lid = 0;
			this->p_wid = 0;
			this->position = position;
			this->speed = speed;
			this->projected_position = projected_position;
			this->color = color;
			this->width = width;
			this->height = height;
			this->inv_mass = inv_mass;
			this->bounceness = bounceness;
			this->is_pinned = is_pinned;
		}
	public:
		size_t add_collide_wid(size_t i) {
			return this->collide_wids.add_item(i);
		}
		size_t add_rev_section(RevSection rsec) {
			return this->rev_sections.add_item(rsec);
		}
	public:
		size_t get_collide_wid_size() {
			return this->collide_wids.size();
		}
		size_t get_rev_section_size() {
			return this->rev_sections.size();
		}
	public:
		size_t get_collide_wid(size_t i) {
			return this->collide_wids[i];
		}
		RevSection get_rev_section(size_t i) {
			return this->rev_sections[i];
		}
};

class Section {
	private:
		Array<Particle *> particles;
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
		size_t add_particle(Particle *pt) {
			return this->particles.add_item(pt);
		}
	public:
		size_t get_particle_size() {
			return this->particles.size();
		}
	public:
		Particle* get_particle(size_t i) {
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
		Array<Particle *> particles;
		Array<Section *> sections;
		
		size_t x,y;
		size_t width,height;
		size_t section_width, section_height;
	public:
		Level() {
			this->x = 0;
			this->y = 0;
			this->width = 0;
			this->height = 0;
			this->section_width = 0;
			this->section_height = 0;
		}
		Level(size_t x, size_t y, size_t width, size_t height, size_t section_width, size_t section_height) {
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
		size_t add_particle(Particle *pt) {
			return this->particles.add_item(pt);
		}
		size_t add_section(Section *sc) {
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
		Particle* get_particle(size_t i) {
			return this->particles[i];
		}
		Section* get_section(size_t i) {
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
	private:
		Array<Particle> particles;
		Array<Section> sections;
		Array<Level> levels;
	public:
		World() {
		}
	public:
		size_t add_level(Level level) {
			size_t level_id;
			size_t section_id;
			level_id = this->levels.add_item(level);
			for (size_t x=0; x < level.width; x++) {
				for (size_t y=0; y < level.height; y++) {
					section_id = this->sections.add_item(Section(x,y));
					this->levels[level_id].add_section(&this->sections[section_id]);
				}
			}
			return level_id;
		}
		size_t add_particle(size_t level_id, Particle pt) {
			size_t wid,lid;
			wid = this->particles.add_item(pt);
			lid = this->levels[level_id].add_particle(&this->particles[wid]);
			this->particles[wid].p_wid = wid;
			this->particles[wid].p_lid = lid;
			this->particles[wid].level_id = level_id;
			return wid;
		}
		size_t add_section(size_t level_id, Section sc) {
			size_t wid;
			wid = this->sections.add_item(sc);
			this->levels[level_id].add_section(&this->sections[wid]);
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
			this->levels.del_item(i);
		}
		void del_particle(size_t i) {
//			this->levels[this->particles[i].level_id].del_particle(this->particles[i].p_lid);
			this->particles.del_item(i);
		}
		void del_section(size_t i) {
			this->sections.del_item(i);
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
			return this->worlds.add_item(world);
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

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1100,600,32,SDL_SWSURFACE);
	Universe universe;
	universe.add_world(World());
	universe.get_world(0)->add_level(Level(0, 0, 500, 500, 20, 20));

/*
	Level level(0, 0, 500, 500, 20, 20);
	Section sec(0,0);
	Particle pt(Vector2(10,10), Vector2(10,10), Vector2(10,10),SDL_MapRGB(screen->format,0xFF,0xFF,0xFF), 10, 10, 1, 0, false);
	sec.add_particle(&pt);
*/
	
	
	return 0;	
}
