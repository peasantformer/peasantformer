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

inline bool operator==(Vector2 l, Vector2 r) {
	return ((l.x == r.x) && (l.y == r.y));
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
typedef long int PeasantPos;
typedef unsigned int PeasantID;
typedef unsigned int PeasantSize;

enum Direction {
	INVALID = 0x0,
	MOVE_UP = 0x1,
	MOVE_DOWN = 0x2,
	MOVE_LEFT = 0x4,
	MOVE_RIGHT = 0x8
};

class Particle {
	private:
		PeasantID id_in_world;
		PeasantID world_id;
		PeasantID u,d,l,r;
		Array <PeasantID> section_ids;
		Array <PeasantID> particle_computed_ids;
		Vector2 position;
		Vector2 speed;
		Vector2 projected_position;
		Uint32 color;
		float width, height;
		float inv_mass;
		bool is_pinned;
	public:
		Particle() {
			this->u = 0;
			this->d = 0;
			this->l = 0;
			this->r = 0;
			this->id_in_world = 0;
			this->world_id = 0;
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
			this->u = 0;
			this->d = 0;
			this->l = 0;
			this->r = 0;
			this->id_in_world = 0;
			this->world_id = 0;
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
		size_t add_section_id(PeasantID value) {
			return this->section_ids.add_item(value);
		}
		size_t add_particle_computed_id(PeasantID value) {
			return this->particle_computed_ids.add_item(value);
		}
	public:
		size_t get_section_id_size() {
			return this->section_ids.size();
		}
		size_t get_particle_computed_id_size() {
			return this->particle_computed_ids.size();
		}
	public:
		PeasantID get_section_id(size_t i) {
			return this->section_ids[i];
		}
		PeasantID get_particle_computed_id(size_t i) {
			return this->particle_computed_ids[i];
		}
	public:
		void clear_section_id(PeasantID value) {
			this->section_ids.clear();
		}
		void clear_particle_computed_id(PeasantID value) {
			this->particle_computed_ids.clear();
		}

	public:
		void set_id_in_world(PeasantID value) {
			this->id_in_world = value;
		}
		void set_world_id(PeasantID value) {
			this->world_id = value;
		}
		void set_u(PeasantID value) {
			this->u = value;
		}
		void set_d(PeasantID value) {
			this->d = value;
		}
		void set_l(PeasantID value) {
			this->l = value;
		}
		void set_r(PeasantID value) {
			this->r = value;
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
		PeasantID get_id_in_world(void) {
			return this->id_in_world;
		}
		PeasantID get_world_id(void) {
			return this->world_id;
		}
		PeasantID get_u(void) {
			return this->u;
		}
		PeasantID get_d(void) {
			return this->d;
		}
		PeasantID get_l(void) {
			return this->l;
		}
		PeasantID get_r(void) {
			return this->r;
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
		bool master_node;
		PeasantPos x,y;
		PeasantSize w,h;
		MaybePeasantID u,d,l,r;
		PeasantID id;
		Array <PeasantID> particle_ids;
	public:
		Section() {
			this->master_node = false;
			this->id = 0;
			this->x = 0;
			this->y = 0;
			this->u = -1;
			this->d = -1;
			this->l = -1;
			this->r = -1;
		}
		Section(PeasantPos x
		       ,PeasantPos y
		       ,MaybePeasantID u = -1
		       ,MaybePeasantID d = -1
		       ,MaybePeasantID l = -1
		       ,MaybePeasantID r = -1
		       ,bool master_node = false) {
			this->master_node = master_node;
			this->id = 0;
			this->x = x;
			this->y = y;
			this->u = u;
			this->d = d;
			this->l = l;
			this->r = r;
		}

	public:
		void set_master_node(bool value) {
			this->master_node = value;
		}
		void set_x(PeasantPos value) {
			this->x = value;
		}
		void set_y(PeasantPos value) {
			this->y = value;
		}
		void set_w(PeasantSize value) {
			this->w = value;
		}
		void set_h(PeasantSize value) {
			this->h = value;
		}
		void set_u(MaybePeasantID value) {
			this->u = value;
		}
		void set_d(MaybePeasantID value) {
			this->d = value;
		}
		void set_l(MaybePeasantID value) {
			this->l = value;
		}
		void set_r(MaybePeasantID value) {
			this->r = value;
		}
		void set_id(PeasantID value) {
			this->id = value;
		}

	public:
		bool get_master_node(void) {
			return this->master_node;
		}
		PeasantPos get_x(void) {
			return this->x;
		}
		PeasantPos get_y(void) {
			return this->y;
		}
		PeasantSize get_w(void) {
			return this->w;
		}
		PeasantSize get_h(void) {
			return this->h;
		}
		MaybePeasantID get_u(void) {
			return this->u;
		}
		MaybePeasantID get_d(void) {
			return this->d;
		}
		MaybePeasantID get_l(void) {
			return this->l;
		}
		MaybePeasantID get_r(void) {
			return this->r;
		}
		PeasantID get_id(void) {
			return this->id;
		}

};

class World {
	private:
		Array <Particle> particles;
		Array <Section> sections;
		PeasantSize section_width;
		PeasantSize section_height;
		PeasantPos max_x, max_y;
		PeasantPos min_x, min_y;
		PeasantID lu,ru,ld,rd;
		bool lu_stright;
		bool ru_stright;
		bool ld_stright;
		bool rd_stright;
	public:
	World(PeasantSize section_width, PeasantSize section_height) {
			this->max_x = 0;
			this->max_y = 0;
			this->min_x = 0;
			this->min_y = 0;
			this->lu = 0;
			this->ru = 0;
			this->ld = 0;
			this->rd = 0;
			this->lu_stright = true;
			this->ru_stright = true;
			this->ld_stright = true;
			this->rd_stright = true;
			this->section_width = section_width;
			this->section_height = section_height;
			this->add_section(Section(0,0,-1,-1,-1,-1,true));
		}
	public:
		size_t add_section(Section sc,PeasantSize sw = 0, PeasantSize sh = 0) {
			if (sw == 0) sw = this->section_width;
			if (sh == 0) sh = this->section_height;
			sc.set_w(sw);
			sc.set_h(sh);

			return this->sections.add_item(sc);
		}
		size_t add_particle(Particle pt) {
			pt.set_world_id(0);
			pt.set_u(0);
			pt.set_d(0);
			pt.set_l(0);
			pt.set_r(0);
			PeasantID id_in_world;
			id_in_world = this->particles.add_item(pt);
			this->get_particle(id_in_world)->set_id_in_world(id_in_world);
			return id_in_world;
		}
	public:
		size_t get_sections_size() {
			return this->sections.size();
		}
		size_t get_particles_size() {
			return this->particles.size();
		}
	public:
		Section *get_section(PeasantID id) {
			return &this->sections[id];
		}
		Particle *get_particle(PeasantID id) {
			return &this->particles[id];
		}
	public:
		bool move_particles() {
			for (size_t i=0; i < this->get_particles_size(); i++) {
				Particle *pt = this->get_particle(i);
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				if (position == projected_position) {
					continue;
				}
				if (position.x != projected_position.x) {
					MaybePeasantID l_id = pt->get_l();
					MaybePeasantID r_id = pt->get_r();
					Section *sc_l = this->get_section(l_id);
					Section *sc_r = this->get_section(r_id);
					
					PeasantSize pt_width = pt->get_width();				
					PeasantPos section_max_x = sc_r->get_x();
					PeasantPos section_min_x = sc_l->get_x();
					
					while ((projected_position.x + pt_width/2) > ((section_max_x+1) * this->section_width)) {
						if (sc_r->get_r() < 0) {
							r_id = gen_new_section(r_id,MOVE_RIGHT);
						}
						if (r_id < 0) {
							break;
						}
						sc_r = this->get_section(r_id);
						section_max_x = sc_r->get_x();
						pt->set_r(r_id);
					}
					
					while ((projected_position.x - pt_width/2) > ((section_min_x+1) * this->section_width)) {
						if (sc_l->get_r() < 0) break;
						l_id = sc_l->get_r();
						sc_l = this->get_section(l_id);
						section_min_x = sc_l->get_x();
					}
					
					
					while ((projected_position.x - pt_width/2) < ((section_min_x) * this->section_width)) {
						if (sc_l->get_l() < 0) {
							l_id = gen_new_section(l_id,MOVE_LEFT);
						}
						if (l_id < 0) {
							break;
						}
						sc_l = this->get_section(l_id);
						section_min_x = sc_l->get_x();
						pt->set_l(l_id);
					}
					
					
					while ((projected_position.x + pt_width/2) < ((section_max_x) * this->section_width)) {
						if (sc_r->get_l() < 0) break;
						r_id = sc_r->get_l();
						sc_r = this->get_section(r_id);
						section_max_x = sc_r->get_x();
					}
					printf("H: %ld - %ld\n",l_id,r_id);
				}
				if (position.y != projected_position.y) {
					MaybePeasantID u_id = pt->get_u();
					MaybePeasantID d_id = pt->get_d();
					Section *sc_u = this->get_section(u_id);
					Section *sc_d = this->get_section(d_id);
					
					PeasantSize pt_height = pt->get_height();
					PeasantPos section_min_y = sc_u->get_y();
					PeasantPos section_max_y = sc_d->get_y();
					
					
					while ((projected_position.y - pt_height/2) < ((section_min_y) * this->section_height)) {
						if (sc_u->get_u() < 0) {
							u_id = gen_new_section(u_id,MOVE_UP);
						} if (u_id < 0) {
							break;
						}
						sc_u = this->get_section(u_id);
						section_min_y = sc_u->get_y();
						pt->set_u(u_id);
					}
					
					while ((projected_position.y + pt_height/2) < ((section_max_y) * this->section_height)) {
						if (sc_d->get_u() < 0) break;
						d_id = sc_d->get_u();
						sc_d = this->get_section(d_id);
						section_max_y = sc_d->get_y();
					}
					
					
					while ((projected_position.y + pt_height/2) > ((section_max_y+1) * this->section_height)) {
						if (sc_d->get_d() < 0) {
							d_id = gen_new_section(d_id,MOVE_DOWN);
						}
						if (d_id < 0) {
							break;
						}
						sc_d = this->get_section(d_id);
						section_max_y = sc_d->get_y();
						pt->set_d(d_id);
					}
					
					while ((projected_position.y - pt_height/2) > ((section_min_y+1) * this->section_height)) {
						if (sc_u->get_d() < 0) break;
						u_id = sc_u->get_d();
						sc_u = this->get_section(u_id);
						section_min_y = sc_u->get_y();
					}
					printf("V: %ld - %ld\n",u_id,d_id);
				}
				pt->set_position(projected_position);
			}
			return true;
		}
		bool place_particle(PeasantID id) {
			return true;
		}
		MaybePeasantID gen_new_section(PeasantID section_id, Direction direction, bool gen = false) {
			Section *current_section = this->get_section(section_id);
			PeasantPos x=0,y=0;
			MaybePeasantID u=0,d=0,l=0,r=0;
			PeasantID id=0,newid=0;
			x = current_section->get_x();
			y = current_section->get_y();
			u = current_section->get_u();
			d = current_section->get_d();
			l = current_section->get_l();
			r = current_section->get_r();
			id = current_section->get_id();
			Section sc(x,y);
			switch (direction) {
				case INVALID:
					return -1;
					break;
				case MOVE_UP:				
					if (current_section->get_u() != -1) {
						printf("u reject with %d\n",current_section->get_u());
						return -1;
					}
					sc.set_y(y-1);
					sc.set_d(id);
					break;
				case MOVE_DOWN:
					if (current_section->get_d() != -1) {
						printf("d reject\n");
						return -1;
					}
					sc.set_y(y+1);
					sc.set_u(id);
					break;
				case MOVE_LEFT:
					if (current_section->get_l() != -1) {
						printf("l reject\n");
						return -1;
					}
					sc.set_x(x-1);
					sc.set_r(id);
					break;
				case MOVE_RIGHT:
					if (current_section->get_r() != -1) {
						printf("r reject\n");
						return -1;
					}
					sc.set_x(x+1);
					sc.set_l(id);
					break;
			}
			newid = this->add_section(sc);
			// we have to aquire NEW pointer to current section, because
			// of vector behaviour - previous values are invalid due to
			// insertion.
			current_section = this->get_section(section_id);
			this->get_section(newid)->set_id(newid);
			switch (direction) {
				case INVALID:
					return -1;
					break;
				case MOVE_UP:
					if (gen == false) {
						this->min_y--;
						if (abs(this->min_y) > abs(this->min_x) && min_x >= 0) {
							this->lu = newid;
							this->lu_stright = false;
						}
						if (abs(this->min_y) > abs(this->max_x) && max_x <= 0) {
							this->ru = newid;
							this->ru_stright = false;
						}	
						MaybePeasantID sect_id = lu;
						MaybePeasantID par_id = newid;
						for (PeasantPos i=this->min_x; i <= this->max_x; i++) {
							if (i == this->min_x && i != 0) {
								if (this->min_x < 0) {
									printf("left\n");
									sect_id = gen_new_section(sect_id,MOVE_UP,true);
									this->lu = sect_id;
								}
								continue;
							}
							if (i == 0) {
								if (this->max_x > 0) {
									sect_id = ru;
									sect_id = gen_new_section(sect_id,MOVE_UP,true);
									this->ru = sect_id;
									if (this->max_x  == 1) {
										this->get_section(section_id)->set_l(newid);
									}
								}
								continue;
							}
							if (i < 0) {
								printf("l %ld\n",i);
								sect_id = gen_new_section(sect_id,MOVE_RIGHT,true);
							} else {
								sect_id = gen_new_section(sect_id,MOVE_LEFT,true);
								printf("r %ld\n",i);
							}
						}
					}
					current_section->set_u(newid);
					break;
				case MOVE_DOWN:
					if (gen == false) {
						this->max_y++;
						if (abs(this->max_y) > abs(this->min_x)) {
							this->ld = newid;
							this->ld_stright = false;
						}
						if (abs(this->max_y) > abs(this->max_x)) {
							this->rd = newid;
							this->rd_stright = false;
						}
					}
					current_section->set_d(newid);
					break;
				case MOVE_LEFT:
					if (gen == false) {
						this->min_x--;
						if (abs(this->min_x) >= abs(this->min_y)) {
							this->lu = newid;
							this->lu_stright = true;
						}
						if (abs(this->min_x) >= abs(this->max_y)) {
							this->ld = newid;
							this->ld_stright = true;
						}
					}
					current_section->set_l(newid);
					break;
				case MOVE_RIGHT:
					if (gen == false) {
						this->max_x++;
						if (abs(this->max_x) >= abs(this->min_y)) {
							this->ru = newid;
							this->ru_stright = true;
						}
						if (abs(this->max_x) >= abs(this->max_y)) {
							this->rd = newid;
							this->rd_stright = true;
						}
					}
					current_section->set_r(newid);
					break;
			}
			return newid;
		}

	public:
		void set_section_width(PeasantSize value) {
			this->section_width = value;
		}
		void set_section_height(PeasantSize value) {
			this->section_height = value;
		}

	public:
		PeasantSize get_section_width(void) {
			return this->section_width;
		}
		PeasantSize get_section_height(void) {
			return this->section_height;
		}

};


class SDLRenderer {
	private:
		SDL_Surface *screen;
		PeasantSize xoffset;
		PeasantSize yoffset;
	public:
		SDLRenderer(SDL_Surface *screen, PeasantSize xoffset = 0, PeasantSize yoffset = 0) {
			this->screen = screen;
			this->xoffset = xoffset;
			this->yoffset = yoffset;
		}
	public:
		void render(void) {
		}
		void render(Section *sc) {
			SDL_Rect rect;
			rect.x = sc->get_x() * sc->get_w() + xoffset;
			rect.y = sc->get_y() * sc->get_h() + yoffset;
			rect.w = sc->get_w();
			rect.h = sc->get_h();
			if (rect.x < 0 || rect.y < 0) return;
			SDL_FillRect(this->screen,&rect,SDL_MapRGB(screen->format,0xFF, 0xFF, 0xFF));
			rect.x += 2;
			rect.y += 2;
			rect.w -= 4;
			rect.h -= 4;
			SDL_FillRect(this->screen,&rect,SDL_MapRGB(screen->format,0xAA, 0xAA, 0xFF));
		}
		void render(Particle *pt) {
			SDL_Rect rect;
			rect.x = pt->get_position().x - pt->get_width()/2 + xoffset;
			rect.y = pt->get_position().y - pt->get_height()/2 + yoffset;
			rect.w = pt->get_width();
			rect.h = pt->get_height();
			if (rect.x < 0 || rect.y < 0) return;
			SDL_FillRect(this->screen,&rect,pt->get_color());
		}
		void redraw() {
			SDL_FillRect(this->screen,&this->screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));
		}
		void flip() {
			SDL_Flip(this->screen);
		}
};




int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1000,700,32,SDL_SWSURFACE);
	
	World world(100,100);
	SDLRenderer render(screen,500,500);
	PeasantID pt_id;
	
	pt_id = world.add_particle(Particle(Vector2(50,50),Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0x00,0x00),50,50,1,false));
	world.place_particle(pt_id);

	bool quit = false; 
	SDL_Event event;
	Uint8 *keystates = SDL_GetKeyState(NULL);
	Uint8 old_keystates[SDLK_LAST];
	memcpy(old_keystates,keystates,SDLK_LAST * sizeof(Uint8));
	Particle *pt = world.get_particle(0);
	while (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) quit = true;

		Vector2 position = pt->get_position();
		
		if (keystates[SDLK_UP]) {
			position += Vector2(0,-1);
		}
		if (keystates[SDLK_DOWN]) {
			position += Vector2(0,1);
		}
		if (keystates[SDLK_LEFT]) {
			position += Vector2(-1,0);
		}
		if (keystates[SDLK_RIGHT]) {
			position += Vector2(1,0);
		}
		
		pt->set_projected_position(position);
		
		world.move_particles();
		
		memcpy(old_keystates,keystates,SDLK_LAST * sizeof(Uint8));
		
		render.redraw();
		for (size_t i=0; i < world.get_sections_size(); i++) {
			render.render(world.get_section(i));
		}
		for (size_t i=0; i < world.get_particles_size(); i++) {
			render.render(world.get_particle(i));
		}
		render.flip();
		SDL_Delay(0);
	}
	
	return 0;
}
