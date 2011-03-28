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

inline bool operator!=(Vector2 l, Vector2 r) {
	return ((l.x != r.x) || (l.y != r.y));
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
				this->add_item(r.data[i]);
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

typedef long int MaybePeasantID;
typedef unsigned int PeasantID;
typedef unsigned int PeasantPos;
typedef unsigned int PeasantSize;

enum Direction {
	NO_MOVE = 0x0,
	MOVE_UP = 0x1,
	MOVE_DOWN = 0x2,
	MOVE_LEFT = 0x4,
	MOVE_RIGHT = 0x8
};

struct ParticleMove {
	PeasantID particle_world_id;
	Vector2 amount;
};

class Particle {
	private:
			PeasantID level_id;
			PeasantID world_id;
			PeasantID levels_id;
			PeasantID worlds_id;
			Array <PeasantID> section_ids;
			Array <PeasantID> computed_ids;
			Vector2 position;
			Vector2 speed;
			Vector2 projected_position;
			Uint32 color;
			float width, height;
			float inv_mass;
			bool is_pinned;
	public:
		Particle() {
			this->levels_id = 0;
			this->worlds_id = 0;
			this->position = Vector2(0,0);
			this->speed = Vector2(0,0);
			this->projected_position = Vector2(0,0);
			this->color = SDL_MapRGB(screen->format,0xFF,0xFF,0xFF);
			this->width = 0;
			this->height = 0;
			this->inv_mass = 0;
			this->is_pinned = false;
		}
		Particle(Vector2 position
		        ,Vector2 speed
		        ,Uint32 color
		        ,float width
		        ,float height
		        ,float inv_mass
		        ,bool is_pinned) {
			this->levels_id = 0;
			this->worlds_id = 0;
			this->position = position;
			this->speed = speed;
			this->projected_position = Vector2(0,0);
			this->color = color;
			this->width = width;
			this->height = height;
			this->inv_mass = inv_mass;
			this->is_pinned = is_pinned;
		}
	public:
		PeasantID add_section_id(PeasantID section_id) {
			PeasantID particle_id;
			particle_id = this->section_ids.add_item(section_id);
			return particle_id;
		}
		PeasantID add_computed_id(PeasantID particle_id) {
			PeasantID computed_id;
			computed_id = this->computed_ids.add_item(particle_id);
			return computed_id;
		}
	public:
		size_t get_section_ids_size() {
			return this->section_ids.size();
		}
		size_t get_computed_ids_size() {
			return this->computed_ids.size();
		}
	public:
		PeasantID get_section_id(size_t id) {
			return this->section_ids[id];
		}
		PeasantID get_computed_id(size_t id) {
			return this->computed_ids[id];
		}
	public:
		void clear_section_ids() {
			this->section_ids.clear();
		}
		void clear_computed_ids() {
			this->computed_ids.clear();
		}

	public:
		void set_level_id(PeasantID value) {
			this->level_id = value;
		}
		void set_world_id(PeasantID value) {
			this->world_id = value;
		}
		void set_levels_id(PeasantID value) {
			this->levels_id = value;
		}
		void set_worlds_id(PeasantID value) {
			this->worlds_id = value;
		}
		void set_position(Vector2 value) {
			this->position = value;
		}
		void set_speed(Vector2 value) {
			this->speed = value;
		}
		void set_projected_position(Vector2 value) {
			this->projected_position = value;
		}
		void set_color(Uint32 value) {
			this->color = value;
		}
		void set_width(float value) {
			this->width = value;
		}
		void set_height(float value) {
			this->height = value;
		}
		void set_inv_mass(float value) {
			this->inv_mass = value;
		}
		void set_is_pinned(bool value) {
			this->is_pinned = value;
		}

	public:
		PeasantID get_level_id(void) {
			return this->level_id;
		}
		PeasantID get_world_id(void) {
			return this->world_id;
		}
		PeasantID get_levels_id(void) {
			return this->levels_id;
		}
		PeasantID get_worlds_id(void) {
			return this->worlds_id;
		}
		Vector2 get_position(void) {
			return this->position;
		}
		Vector2 get_speed(void) {
			return this->speed;
		}
		Vector2 get_projected_position(void) {
			return this->projected_position;
		}
		Uint32 get_color(void) {
			return this->color;
		}
		float get_width(void) {
			return this->width;
		}
		float get_height(void) {
			return this->height;
		}
		float get_inv_mass(void) {
			return this->inv_mass;
		}
		bool get_is_pinned(void) {
			return this->is_pinned;
		}

};

class Section {
	private:
		PeasantID level_id;
		PeasantID world_id;
		PeasantID levels_id;
		PeasantID worlds_id;

		Array <PeasantID> particle_ids;
		PeasantPos x,y;
	public:
		Section() {
			this->level_id = 0;
			this->x = 0;
			this->y = 0;
		}
		Section(PeasantPos x
		       ,PeasantPos y) {
			this->level_id = 0;
			this->x = x;
			this->y = y;
		}
	public:
		size_t get_particle_ids_size() {
			return this->particle_ids.size();
		}
	public:
		PeasantID add_particle_id(PeasantID value) {
			PeasantID section_id;
			section_id = this->particle_ids.add_item(value);
			return section_id;
		}
	public:
		PeasantID get_particle_id(size_t value) {
			return this->particle_ids[value];
		}
	public:
		void del_particle_by_id(size_t id) {
			for (size_t i=0; i < this->particle_ids.size(); i++) {
				if (particle_ids[i] == id) {
					this->particle_ids.del_item(i);
					printf(".\n");
					return;
				}
			}
		}
	public:
		void clear_particle_ids() {
			this->particle_ids.clear();
		}

	public:
		void set_level_id(PeasantID value) {
			this->level_id = value;
		}
		void set_world_id(PeasantID value) {
			this->world_id = value;
		}
		void set_levels_id(PeasantID value) {
			this->levels_id = value;
		}
		void set_worlds_id(PeasantID value) {
			this->worlds_id = value;
		}
		void set_x(PeasantPos value) {
			this->x = value;
		}
		void set_y(PeasantPos value) {
			this->y = value;
		}

	public:
		PeasantID get_level_id(void) {
			return this->level_id;
		}
		PeasantID get_world_id(void) {
			return this->world_id;
		}
		PeasantID get_levels_id(void) {
			return this->levels_id;
		}
		PeasantID get_worlds_id(void) {
			return this->worlds_id;
		}
		PeasantPos get_x(void) {
			return this->x;
		}
		PeasantPos get_y(void) {
			return this->y;
		}

};

class Level {
	private:
		PeasantID world_id;
		PeasantID worlds_id;
		Array <PeasantID> section_ids;
		Array <PeasantID> particle_ids;
		
		PeasantPos x,y;
		PeasantSize width,height;
		PeasantSize section_width,section_height;
		PeasantSize sections_x;
		PeasantSize sections_y;
	public:
		Level() {
			this->worlds_id = 0;
			this->x = 0;
			this->y = 0;
			this->width = 0;
			this->height = 0;
			this->section_width = 0;
			this->section_height = 0;
			this->sections_x = 0;
			this->sections_y = 0;
		}
		Level(PeasantPos x
		     ,PeasantPos y
		     ,PeasantSize width
		     ,PeasantSize height
		     ,PeasantSize section_width
		     ,PeasantSize section_height) {
			this->worlds_id = 0;
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			this->section_width = section_width;
			this->section_height = section_height;
			this->sections_x = 0;
			this->sections_y = 0;
		}
	public:
		PeasantID add_section_id(PeasantID value) {
			PeasantID level_id;
			level_id = this->section_ids.add_item(value);
			return level_id;
		}
		PeasantID add_particle_id(PeasantID value) {
			PeasantID level_id;
			level_id = this->particle_ids.add_item(value);
			return level_id;
		}
	public:
		void del_particle_at(size_t id) {
			this->particle_ids.del_item(id);
		}
	public:
		size_t get_section_ids_size() {
			return this->section_ids.size();
		}
		size_t get_particle_ids_size() {
			return this->particle_ids.size();
		}
	public:
		PeasantID get_section_id(size_t i) {
			return this->section_ids[i];
		}
		PeasantID get_particle_id(size_t i) {
			return this->particle_ids[i];
		}
	public:
		bool  is_valid_particle_by_id(PeasantID id) {
			for (PeasantID i=0; i < this->particle_ids.size(); i++) {
				if (this->particle_ids[i] == id) {
					return true;
				}
			}
			return false;
		}

	public:
		void set_world_id(PeasantID value) {
			this->world_id = value;
		}
		void set_worlds_id(PeasantID value) {
			this->worlds_id = value;
		}
		void set_x(PeasantPos value) {
			this->x = value;
		}
		void set_y(PeasantPos value) {
			this->y = value;
		}
		void set_width(PeasantSize value) {
			this->width = value;
		}
		void set_height(PeasantSize value) {
			this->height = value;
		}
		void set_section_width(PeasantSize value) {
			this->section_width = value;
		}
		void set_section_height(PeasantSize value) {
			this->section_height = value;
		}
		void set_sections_x(PeasantSize value) {
			this->sections_x = value;
		}
		void set_sections_y(PeasantSize value) {
			this->sections_y = value;
		}

	public:
		PeasantID get_world_id(void) {
			return this->world_id;
		}
		PeasantID get_worlds_id(void) {
			return this->worlds_id;
		}
		PeasantPos get_x(void) {
			return this->x;
		}
		PeasantPos get_y(void) {
			return this->y;
		}
		PeasantSize get_width(void) {
			return this->width;
		}
		PeasantSize get_height(void) {
			return this->height;
		}
		PeasantSize get_section_width(void) {
			return this->section_width;
		}
		PeasantSize get_section_height(void) {
			return this->section_height;
		}
		PeasantSize get_sections_x(void) {
			return this->sections_x;
		}
		PeasantSize get_sections_y(void) {
			return this->sections_y;
		}

};

class World {
	private:
		PeasantID universe_id;
		PeasantID universes_id;
		Array <Particle> particles;
		Array <Section> sections;
		Array <Level> levels;
		Array <PeasantID> computed_ids;
	public:
		World() {
			this->universe_id = 0;
			this->universes_id = 0;
		}
	public:
		PeasantID add_particle(PeasantID level_id, Particle pt) {
			PeasantID worlds_id;
			worlds_id = this->particles.add_item(pt);
			this->particles[worlds_id].set_worlds_id(worlds_id);
			this->particles[worlds_id].set_world_id(this->universes_id);
			return worlds_id;
			
		}
		PeasantID add_section(PeasantID level_id, Section sc) {
			PeasantID worlds_id;
			worlds_id = this->sections.add_item(sc);
			this->sections[worlds_id].set_worlds_id(worlds_id);
			this->sections[worlds_id].set_world_id(this->universes_id);
			return worlds_id;
		}
		PeasantID add_level(Level lv) {
			PeasantID worlds_id;
			worlds_id = this->levels.add_item(lv);
			this->levels[worlds_id].set_worlds_id(worlds_id);
			this->levels[worlds_id].set_world_id(this->universes_id);
			return worlds_id;
		}
	public:
		size_t get_particles_size() {
			return this->particles.size();
		}
		size_t get_levels_size() {
			return this->levels.size();
		}
		size_t get_sections_size() {
			return this->sections.size();
		}
	public:
		Particle *get_particle(PeasantID pt_id) {
			return &this->particles[pt_id];
		}
		Level *get_level(PeasantID lvl_id) {
			return &this->levels[lvl_id];
		}
		Section *get_section(PeasantID sc_id) {
			return &this->sections[sc_id];
		}
	public:
		bool  is_valid_level_by_id(PeasantID id) {
			for (PeasantID i=0; i < this->levels.size(); i++) {
				if (this->levels[i].get_worlds_id() == id) {
					return true;
				}
			}
			return false;
		}
		bool  is_valid_section_by_id(PeasantID id) {
			for (PeasantID i=0; i < this->sections.size(); i++) {
				if (this->sections[i].get_worlds_id() == id) {
					return true;
				}
			}
			return false;
		}
		bool  is_valid_particle_by_id(PeasantID id) {
			for (PeasantID i=0; i < this->particles.size(); i++) {
				if (this->particles[i].get_worlds_id() == id) {
					return true;
				}
			}
			return false;
		}
	public:
		MaybePeasantID link_section_with_level(PeasantID level_id, PeasantID section_id) {
			/*
			if (!this->is_valid_level_by_id(level_id) || !this->is_valid_section_by_id(section_id)) {
				return -1;
			}
			*/
			PeasantID levels_id;
			levels_id = this->levels[level_id].add_section_id(section_id);
			this->sections[levels_id].set_levels_id(levels_id);
			this->sections[levels_id].set_level_id(level_id);
			return levels_id;
		}
		MaybePeasantID link_particle_with_level(PeasantID level_id, PeasantID particle_id) {
			/*
			if (!this->is_valid_level_by_id(level_id) || !this->is_valid_particle_by_id(particle_id)) {
				return -1;
			}
			*/
			PeasantID levels_id;
			levels_id = this->levels[level_id].add_particle_id(particle_id);
			this->particles[levels_id].set_levels_id(levels_id);
			this->particles[levels_id].set_level_id(level_id);
			return levels_id;
		}
		MaybePeasantID link_particle_with_section(PeasantID section_id, PeasantID particle_id) {
			/*
			if (!this->is_valid_section_by_id(section_id) || !this->is_valid_particle_by_id(particle_id)) {
				return -1;
			}
			*/
			PeasantID sections_id;
			sections_id = this->sections[section_id].add_particle_id(particle_id);
			this->particles[sections_id].add_section_id(sections_id);
			return sections_id;
		}
	public:
		bool unlink_particle_with_level(PeasantID level_id, PeasantID particle_level_id) {
			this->levels[level_id].del_particle_at(particle_level_id);
			return true;
		}
	public:
		bool gen_sections_for_level(PeasantID level_id) {
			PeasantSize sections_x = 0;
			PeasantSize sections_y = 0;
			PeasantSize level_width = this->levels[level_id].get_width();
			PeasantSize level_height = this->levels[level_id].get_height();
			PeasantSize level_section_width = this->levels[level_id].get_section_width();
			PeasantSize level_section_height = this->levels[level_id].get_section_height();
			
			PeasantID section_id;
			bool ret;
			
			sections_x = level_width / level_section_width;
			sections_y = level_height / level_section_height;
			
			this->levels[level_id].set_sections_x(sections_x);
			this->levels[level_id].set_sections_y(sections_y);
			
			/*
			if (level_width % level_section_width) sections_x += 1;
			if (level_height % level_section_height) sections_y += 1;
			*/
			
			for (PeasantPos y=0; y < sections_y; y++) {
				for (PeasantPos x=0; x < sections_x; x++) {
					section_id = this->add_section(level_id,Section(x,y));
					ret = this->link_section_with_level(level_id,section_id);
					if (ret < 0) {
						return false;
					}
				}
			}
			return true;
		}
		bool gen_particle_links_with_level_sections(PeasantID level_id, PeasantID particle_id) {
			Particle *pt = &this->particles[particle_id];
			Level *lvl = &this->levels[level_id];
			if (!lvl->is_valid_particle_by_id(particle_id)) {
				return false;
			}
			Vector2 pt_position = pt->get_position();
			PeasantSize pt_width = pt->get_width();
			PeasantSize pt_height = pt->get_height();
			PeasantSize lvl_sec_width = lvl->get_section_width();
			PeasantSize lvl_sec_height = lvl->get_section_height();
			
			/* absolute */
			PeasantPos x_start_abs = (pt_position.x - (pt_width /2))  / lvl_sec_width;
			PeasantPos y_start_abs = (pt_position.y - (pt_height/2))  / lvl_sec_height;
			/* relative */
			PeasantPos x_end_rel   = (pt_position.x + (pt_width /2))  / lvl_sec_width - x_start_abs;
			PeasantPos y_end_rel   = (pt_position.y + (pt_height/2))  / lvl_sec_height - y_start_abs;
			
			size_t sc_level_id = 0;
			PeasantID sc_id = 0;
			
			size_t offset = 0;
			
			bool found = false;
			
			size_t section_ids_size = lvl->get_section_ids_size();
			for (size_t i=0; i < section_ids_size; i++) {
				sc_id = lvl->get_section_id(i);
				if (this->sections[sc_id].get_x() == x_start_abs && this->sections[sc_id].get_y() == y_start_abs) {
					offset = i;
					found = true;
					break;
				}
			}
			if (!found) return false;
			
			this->particles[particle_id].clear_section_ids();
			
			for (size_t y=0; y <= y_end_rel; y++) {
				for (size_t x=0; x <= x_end_rel; x++) {
					sc_level_id = offset + y * lvl->get_sections_y() + x;
					sc_id = lvl->get_section_id(sc_level_id);
					this->sections[sc_id].add_particle_id(particle_id);
					this->particles[particle_id].add_section_id(sc_id);
				}
			}
			return true;
		}
		bool clear_particle_section_links() {
			for (size_t sec_id=0; sec_id < this->sections.size(); sec_id++) {
				this->sections[sec_id].clear_particle_ids();
			}
			for (size_t pt_id=0; pt_id < this->particles.size(); pt_id++) {
				this->particles[pt_id].clear_section_ids();
			}
			return true;
		}
		bool gen_particle_section_links() {
			for (size_t lvl_id=0; lvl_id < this->levels.size(); lvl_id++) {
				for (size_t pt_id=0; pt_id < this->levels[lvl_id].get_particle_ids_size(); pt_id++) {
					this->gen_particle_links_with_level_sections(lvl_id,this->levels[lvl_id].get_particle_id(pt_id));
				}
			}
			return true;
		}
		bool move_particle_section_links(Array <ParticleMove> moves, bool delete_on_outbounds = false) {
			for (size_t i=0; i < moves.size(); i++) {
				PeasantID pt_id = moves[i].particle_world_id;
				Particle *pt = this->get_particle(pt_id);
				PeasantID lvl_id = pt->get_level_id();
				PeasantID lvls_id = pt->get_levels_id();
				Level *lvl = this->get_level(lvl_id);
				lvl->del_particle_at(lvls_id);
				Vector2 pt_position = pt->get_position();
				for (size_t i=0; i < pt->get_section_ids_size(); i++) {
					PeasantID sc_id = pt->get_section_id(i);
					Section *sc = &this->sections[sc_id];
					sc->del_particle_by_id(pt_id);
				}
				
				/*
				
				
				
				//lvl->del_particle_at(
				
				
				if (moves[i].amount.x != 0) {
					PeasantSize pt_width = pt->get_width();
					PeasantSize lvl_sec_width = lvl->get_section_width();
					PeasantPos x_start_abs = (pt_position.x - (pt_width /2))  / lvl_sec_width;
					PeasantPos x_end_rel   = (pt_position.x + (pt_width /2))  / lvl_sec_width - x_start_abs;

				}
				if (moves[i].amount.y != 0) {
				}
				*/
			}
			//printf("%d\n",moves.size());	
			/*
			
				if ((moves[i].direction & MOVE_UP) && (moves[i].direction & MOVE_DOWN)) {
					return false;
				}
				if ((moves[i].direction & MOVE_LEFT) && (moves[i].direction & MOVE_RIGHT)) {
					return false;
				}
			}
			*/
			return true;	
		}
	public:
		void set_universe_id(PeasantID value) {
			this->universe_id = value;
		}	

	public:
		PeasantID get_universe_id(void) {
			return this->universe_id;
		}

};

class SDLRenderer {
	private:
		SDL_Surface *screen;
	public:
		SDLRenderer(SDL_Surface *screen) {
			this->screen = screen;
		}
	public:
		void render(void) {
		}
		void render(Particle *pt) {
			SDL_Rect rect;
			rect.x = pt->get_position().x - pt->get_width()/2;
			rect.y = pt->get_position().y - pt->get_height()/2;
			rect.w = pt->get_width();
			rect.h = pt->get_height();
			SDL_FillRect(this->screen,&rect,pt->get_color());
		}
		void redraw() {
			SDL_FillRect(this->screen,&this->screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));
		}
		void flip() {
			SDL_Flip(this->screen);
		}
};

class SingleThreadedPhysComputor {
	private:
		World *world;
		Vector2 gravity;
		float dt;
	public:
		SingleThreadedPhysComputor(World *world) {
			this->world = world;
			this->gravity = Vector2(0,0.1);
			this->dt = 0.01;
		}
	public:
		Array <ParticleMove> compute() {
			Array <ParticleMove> moves;
			for (size_t i=0; i < this->world->get_particles_size(); i++) {
				Particle *pt = this->world->get_particle(i);
				if (pt->get_is_pinned()) continue;
				pt->clear_computed_ids();
				
				Vector2 speed = pt->get_speed();
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				speed += gravity * dt;
				projected_position = position + speed;
				
				pt->set_speed(speed);
				pt->set_projected_position(projected_position);
			}
//					PeasantID lvl_id = sc->get_level_id();
//					Level *lvl = this->world->get_level(lvl_id);
/*
			for (size_t i=0; i < this->world->get_sections_size(); i++) {
				Section *sc = this->world->get_section(i);
				for (size_t n=0; n < sc->get_particle_ids_size(); n++) {
					PeasantID pt_id = sc->get_particle_id(n);
					if (this->world->was_computed(pt_id)) continue;
					Particle *pt = this->world->get_particle(pt_id);
					Vector2 position = pt->get_position();
					position.y += 1;
					pt->set_position(position);
					this->world->add_computed_id(pt_id);
				}
			}
*/
			for (size_t i=0; i < this->world->get_particles_size(); i++) {
				Particle *pt = this->world->get_particle(i);
				if (pt->get_is_pinned()) continue;
				Vector2 speed = pt->get_speed();
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				speed = projected_position - position;
				if (speed != Vector2(0,0)) {
					ParticleMove move;
					move.particle_world_id = pt->get_world_id();
					move.amount = speed;
					moves.add_item(move);
				}
				pt->set_speed(speed);
				pt->set_position(projected_position);
			}

			return moves;
		}
};


int main(int argc, char **argv) {
	bool ret;
	World world;
	PeasantID level_id;
	PeasantID particle_id;
	SingleThreadedPhysComputor phys(&world);
	Array <ParticleMove> moves;
	
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1000,700,32,SDL_SWSURFACE);
	SDLRenderer render(screen);
	
	level_id = world.add_level(Level(0,0,1000,600,20,20));
	
	ret = world.gen_sections_for_level(level_id);
	if (!ret) {
		printf("Failed to generate sections for level\n");
		return -1;
	}

	for (size_t y=0; y < 50; y++) {
		for (size_t x=0; x < 100; x++) {
			particle_id = world.add_particle(level_id,Particle(Vector2(10+x*7,100+y*7),Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0x00,0x00),5,5,1,false));
			ret = (world.link_particle_with_level(level_id,particle_id) >= 0);
			if (!ret) {
				printf("Failed to link particle with level\n");
				return -1;
			}
		}
	}
	
	bool quit = false; 
	SDL_Event event;
	
	printf("test\n");

	world.clear_particle_section_links();
	world.gen_particle_section_links();	

	/* main loop */
	while (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) quit = true;
		
		Uint32 ticks = SDL_GetTicks();
		moves = phys.compute();
		
		ret = world.move_particle_section_links(moves);
		if (!ret) {
			printf("Failed to move particles. Abroting\n");
			exit(1);
		}
		printf("TICKS @ %d\n",SDL_GetTicks() - ticks);
		
		moves.clear();
		
		render.redraw();
		for (size_t i=0; i < world.get_particles_size(); i++) {
			render.render(world.get_particle(i));
		}
		render.flip();
		SDL_Delay(0);
	}
	
	return 0;
}
