#include <stdio.h>
#include <cmath>
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

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

typedef long int PeasantPos;
typedef int64_t PeasantID;
typedef unsigned int PeasantSize;

enum Direction {
	INVALID = 0x0,
	MOVE_UP = 0x1,
	MOVE_DOWN = 0x2,
	MOVE_LEFT = 0x4,
	MOVE_RIGHT = 0x8
};

class Section;

class Particle {
	private:
		PeasantID id;
		PeasantID u;
		PeasantID d;
		PeasantID l;
		PeasantID r;
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
			this->id = 0;
			this->u = 0;
			this->d = 0;
			this->l = 0;
			this->r = 0;
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
			this->id = 0;
			this->u = 0;
			this->d = 0;
			this->l = 0;
			this->r = 0;
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
		void set_id(PeasantID value) {
			this->id = value;
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
		PeasantID get_id(void) {
			return this->id;
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
		PeasantID id;
		
		PeasantPos x;
		PeasantPos y;
		PeasantSize w;
		PeasantSize h;
		
		PeasantID u;
		PeasantID d;
		PeasantID l;
		PeasantID r;
		
		Array <PeasantID> particle_ids;
	public:
		Section() {
			this->id = 0;
			this->x = 0;
			this->y = 0;
			this->w = 0;
			this->h = 0;
			this->u = -1;
			this->d = -1;
			this->l = -1;
			this->r = -1;
		}
		Section(PeasantPos x
		       ,PeasantPos y
		       ,PeasantID u = -1
		       ,PeasantID d = -1
		       ,PeasantID l = -1
		       ,PeasantID r = -1) {
			this->id = 0;
			this->x = x;
			this->y = y;
			this->w = 0;
			this->h = 0;
			this->u = u;
			this->d = d;
			this->l = l;
			this->r = r;
		}

	public:
		void set_id(PeasantID value) {
			this->id = value;
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

	public:
		PeasantID get_id(void) {
			return this->id;
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

};


class World {
	private:
		Array <Particle> particles;
		Array <Section> sections;
		PeasantSize s_w;
		PeasantSize s_h;
		PeasantID u;
		PeasantID d;
		PeasantID l;
		PeasantID r;
	public:
		World(PeasantSize s_w, PeasantSize s_h) {
			this->u = 0;
			this->d = 0;
			this->l = 0;
			this->r = 0;
			this->s_w = s_w;
			this->s_h = s_h;
			this->add_section(Section(0,0,-1,-1,-1,-1));
		}

	public:
		PeasantID add_section(Section sc) {
			sc.set_w(s_w);
			sc.set_h(s_h);
			PeasantID id;
			id = (PeasantID)this->sections.add_item(sc);
			this->get_section(id)->set_id(id);
			return (PeasantID)id;
		}
		PeasantID add_particle(Particle pt) {
			PeasantID id;
			id = (PeasantID)this->particles.add_item(pt);
			this->get_particle(id)->set_id((PeasantID)id);
			return (PeasantID)id;
		}
	public:
		PeasantID get_sections_size() {
			return (PeasantID)this->sections.size();
		}
		PeasantID get_particles_size() {
			return (PeasantID)this->particles.size();
		}
	public:
		Section *get_section(PeasantID id) {
			return &this->sections[(size_t)id];
		}
		Particle *get_particle(PeasantID id) {
			return &this->particles[(size_t)id];
		}
	public:
		bool move_particles() {
			for (PeasantID i=0; i < this->get_particles_size(); i++) {
				Particle *pt = this->get_particle(i);
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				if (position == projected_position) {
					continue;
				}
				PeasantSize pt_width = pt->get_width();				
				PeasantSize pt_height = pt->get_height();

				PeasantID u_id = this->get_u();
				PeasantID d_id = this->get_d();
				PeasantID l_id = this->get_l();
				PeasantID r_id = this->get_r();
				Section *sc_u = this->get_section(u_id);
				Section *sc_d = this->get_section(d_id);
				Section *sc_l = this->get_section(l_id);
				Section *sc_r = this->get_section(r_id);
				PeasantPos section_max_x = sc_r->get_x();
				PeasantPos section_min_x = sc_l->get_x();
				PeasantPos section_min_y = sc_u->get_y();
				PeasantPos section_max_y = sc_d->get_y();
				
				PeasantID p_u_id = pt->get_u();
				PeasantID p_d_id = pt->get_d();
				PeasantID p_l_id = pt->get_l();
				PeasantID p_r_id = pt->get_r();				
				Section *pt_sc_u = this->get_section(p_u_id);
				Section *pt_sc_d = this->get_section(p_d_id);
				Section *pt_sc_l = this->get_section(p_l_id);
				Section *pt_sc_r = this->get_section(p_r_id);
				PeasantPos p_section_max_x = pt_sc_r->get_x();
				PeasantPos p_section_min_x = pt_sc_l->get_x();
				PeasantPos p_section_min_y = pt_sc_u->get_y();
				PeasantPos p_section_max_y = pt_sc_d->get_y();
				
				
				if (position.x != projected_position.x) {
					while ((projected_position.x + pt_width/2) > ((section_max_x+1) * this->s_w)) {
						if (sc_r->get_r() < 0) {
							r_id = gen_new_section(r_id,MOVE_RIGHT);
							if (r_id < 0) {
								break;
							}
							sc_r = this->get_section(r_id);
							section_max_x = sc_r->get_x();
							this->set_r(r_id);
						}
						
					}
					
					while ((projected_position.x - pt_width/2) < ((section_min_x) * this->s_w)) {
						if (sc_l->get_l() < 0) {
							l_id = gen_new_section(l_id,MOVE_LEFT);
							if (l_id < 0) {
								break;
							}
							sc_l = this->get_section(l_id);
							section_min_x = sc_l->get_x();
							this->set_l(l_id);
						}
					}
					
					while ((projected_position.x + pt_width/2) > ((p_section_max_x+1) * this->s_w)) {
						if (pt_sc_r->get_r() < 0) break;
						p_r_id = pt_sc_r->get_r();
						pt_sc_r = this->get_section(p_r_id);
						p_section_max_x = pt_sc_r->get_x();
						pt->set_r(p_r_id);
					}
					while ((projected_position.x + pt_width/2) < ((p_section_max_x) * this->s_w)) {
						if (pt_sc_r->get_l() < 0) break;
						p_r_id = pt_sc_r->get_l();
						pt_sc_r = this->get_section(p_r_id);
						p_section_max_x = pt_sc_r->get_x();
						pt->set_r(p_r_id);
					}
					
					while ((projected_position.x - pt_width/2) < ((p_section_min_x) * this->s_w)) {
						if (pt_sc_l->get_l() < 0) break;
						p_l_id = pt_sc_l->get_l();
						pt_sc_l = this->get_section(p_l_id);
						p_section_min_x = pt_sc_l->get_x();
						pt->set_l(p_l_id);
					}
					
					while ((projected_position.x - pt_width/2) > ((p_section_min_x+1) * this->s_w)) {
						if (pt_sc_l->get_r() < 0) break;
						p_l_id = pt_sc_l->get_r();
						pt_sc_l = this->get_section(p_l_id);
						p_section_min_x = pt_sc_l->get_x();
						pt->set_l(p_l_id);
					}
					

					
				}
				if (position.y != projected_position.y) {
					while ((projected_position.y - pt_height/2) < ((section_min_y) * this->s_h)) {
						if (sc_u->get_u() < 0) {
							u_id = gen_new_section(u_id,MOVE_UP);
							 if (u_id < 0) {
								break;
							}
							sc_u = this->get_section(u_id);
							section_min_y = sc_u->get_y();
							this->set_u(u_id);
						}
					}
					
					while ((projected_position.y + pt_height/2) > ((section_max_y+1) * this->s_h)) {
						if (sc_d->get_d() < 0) {
							d_id = gen_new_section(d_id,MOVE_DOWN);
							if (d_id < 0) {
								break;
							}
							sc_d = this->get_section(d_id);
							section_max_y = sc_d->get_y();
							this->set_d(d_id);
						}
						
					}
					
					while ((projected_position.y - pt_height/2) < ((p_section_min_y) * this->s_h)) {
						if (pt_sc_u->get_u() < 0) break;
						p_u_id = pt_sc_u->get_u();
						pt_sc_u = this->get_section(p_u_id);
						p_section_min_y = pt_sc_u->get_y();
						pt->set_u(p_u_id);
						p_r_id = pt_sc_r->get_u();
						p_l_id = pt_sc_l->get_u();
						pt->set_r(p_r_id);
						pt->set_l(p_l_id);						
					}
					
					while ((projected_position.y - pt_height/2) > ((p_section_min_y+1) * this->s_h)) {
						if (pt_sc_u->get_d() < 0) break;
						p_u_id = pt_sc_u->get_d();
						pt_sc_u = this->get_section(p_u_id);
						p_section_min_y = pt_sc_u->get_y();
						pt->set_u(p_u_id);
						p_r_id = pt_sc_r->get_d();
						p_l_id = pt_sc_l->get_d();
						pt->set_r(p_r_id);
						pt->set_l(p_l_id);
					}
					
					while ((projected_position.y + pt_height/2) > ((p_section_max_y+1) * this->s_h)) {
						if (pt_sc_d->get_d() < 0) break;
						p_d_id = pt_sc_d->get_d();
						pt_sc_d = this->get_section(p_d_id);
						p_section_max_y = pt_sc_d->get_y();
						pt->set_d(p_d_id);
						p_r_id = pt_sc_r->get_d();
						p_l_id = pt_sc_l->get_d();
						pt->set_r(p_r_id);
						pt->set_l(p_l_id);
					}
					
					while ((projected_position.y + pt_height/2) < ((p_section_max_y) * this->s_h)) {
						if (pt_sc_d->get_u() < 0) break;
						p_d_id = pt_sc_d->get_u();
						pt_sc_d = this->get_section(p_d_id);
						p_section_max_y = pt_sc_d->get_y();
						pt->set_d(p_d_id);
						p_r_id = pt_sc_r->get_u();
						p_l_id = pt_sc_l->get_u();
						pt->set_r(p_r_id);
						pt->set_l(p_l_id);
					}
					
				}
				pt->set_position(projected_position);
				printf("%d - %d - %d - %d\n",p_l_id,p_r_id,p_u_id,p_d_id);
			}
			return true;
		}
		PeasantID gen_new_section(PeasantID cid, Direction direction, bool gen = false) {
			Section *current_section = this->get_section(cid);
			PeasantPos x = current_section->get_x();
			PeasantPos y = current_section->get_y();
			Section sc(x,y);
			switch (direction) {
				case INVALID:
					return -1;
					break;
				case MOVE_UP:
					if (current_section->get_u() != -1) {
						return -1;
					}
					sc.set_y(y-1);
					sc.set_d(cid);
					break;
				case MOVE_DOWN:
					if (current_section->get_d() != -1) {
						return -1;
					}
					sc.set_y(y+1);
					sc.set_u(cid);
					break;
				case MOVE_LEFT:
					if (current_section->get_l() != -1) {
						return -1;
					}
					sc.set_x(x-1);
					sc.set_r(cid);
					break;
				case MOVE_RIGHT:
					if (current_section->get_r() != -1) {
						return -1;
					}
					sc.set_x(x+1);
					sc.set_l(cid);
					break;
			}
			
			PeasantID newid = this->add_section(sc);
			// we have to aquire NEW pointer to current section, because
			// of vector behaviour - previous values are invalid due to
			// insertion.
			current_section = this->get_section(cid);
			PeasantID sect_id;
			PeasantID neigh_id;
			Section *neigh;
			Section *sect;
			
			switch (direction) {
				case INVALID:
					return -1;
					break;
				case MOVE_UP:
					if (current_section->get_u() != -1) {
						return -1;
					}
					current_section->set_u(newid);
					if (gen == false) {
						sect_id = newid;
						neigh_id = current_section->get_l();
						neigh = this->get_section(neigh_id);
						while (neigh_id != -1) {
							sect_id = gen_new_section(sect_id,MOVE_LEFT,true);
							neigh->set_u(sect_id);
							sect = this->get_section(sect_id);
							sect->set_d(neigh_id);
							neigh_id = neigh->get_l();
							neigh = this->get_section(neigh_id);
						}
						sect_id = newid;
						neigh_id = current_section->get_r();
						neigh = this->get_section(neigh_id);
						while (neigh_id != -1) {
							sect_id = gen_new_section(sect_id,MOVE_RIGHT,true);
							neigh->set_u(sect_id);
							sect = this->get_section(sect_id);
							sect->set_d(neigh_id);
							neigh_id = neigh->get_r();
							neigh = this->get_section(neigh_id);
						}
					}
					break;
				case MOVE_DOWN:
					if (current_section->get_d() != -1) {
						return -1;
					}
					current_section->set_d(newid);
					break;
				case MOVE_LEFT:
					if (current_section->get_l() != -1) {
						return -1;
					}
					current_section->set_l(newid);
					break;
				case MOVE_RIGHT:
					if (current_section->get_r() != -1) {
						return -1;
					}
					current_section->set_r(newid);
					break;
			}
			return newid;
		}

	public:
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

	public:
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

};


class SDLRenderer {
	private:
		SDL_Surface *screen;
		PeasantSize xoffset;
		PeasantSize yoffset;
		TTF_Font *font ;
		SDL_Color text_color;
		
	public:
		SDLRenderer(SDL_Surface *screen, PeasantSize xoffset = 0, PeasantSize yoffset = 0) {
			this->screen = screen;
			this->xoffset = xoffset;
			this->yoffset = yoffset;
			this->font = TTF_OpenFont( "/usr/share/fonts/liberation-fonts/LiberationMono-Regular.ttf", 28 );
			this->text_color.r=0xFF;
			this->text_color.g=0xFF;
			this->text_color.b=0xFF;
		}
	public:
		void render(void) {
		}
		void render(Section *sc) {
			SDL_Surface *message = NULL;
			char number[1024];
			snprintf(number,1023,"%d",sc->get_id());
			message = TTF_RenderText_Solid( font, number, text_color );
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
			rect.x += rect.w/2-strlen(number);
			rect.y += rect.w/2-14;
			SDL_BlitSurface(message, NULL, screen, &rect );
			SDL_FreeSurface(message);
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
	TTF_Init();

	World world(100,100);
	SDLRenderer render(screen,500,500);
	
	PeasantID pt_id;
	pt_id = world.add_particle(Particle(Vector2(50,50),Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0x00,0x00),50,50,1,false));

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
		for (PeasantID i=0; i < world.get_sections_size(); i++) {
			render.render(world.get_section(i));
		}
		for (PeasantID i=0; i < world.get_particles_size(); i++) {
			render.render(world.get_particle(i));
		}
		render.flip();
		SDL_Delay(0);
	}
	return 0;
}
