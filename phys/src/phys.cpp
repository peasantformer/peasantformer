#include <stdio.h>
#include <cmath>
#include <map>
#include <list>
#include "SDL/SDL.h"
#include "SDL/SDL_ttf.h"

SDL_Surface *screen;

typedef long int PeasantPos;
typedef int64_t PeasantID;
typedef unsigned int PeasantSize;

class Vector2;
class Section;
class Particle;

enum Direction {
	INVALID = 0x0,
	MOVE_UP = 0x1,
	MOVE_DOWN = 0x2,
	MOVE_LEFT = 0x4,
	MOVE_RIGHT = 0x8
};


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
		inline Vector2 abs_normalize() {
			float len = this->length();
			if (len == 0)
				return Vector2(0,0);
			float inv_length = 1.0f / len;
			Vector2 norm = ((*this) * inv_length);
			float bound = 0.7;
//			printf("I: %f %f\n",norm.x,norm.y);
			if (norm.y > 0) {
				if (norm.y > bound) norm.y = 1;
				if (norm.y < bound) norm.y = 0;

			} else {
				if (norm.y < -bound) norm.y = -1;
				if (norm.y > -bound) norm.y = 0;
			}
			if (norm.x > 0) {
				if (norm.x > bound) norm.x = 1;
				if (norm.x < bound) norm.x = 0;
			} else {
				if (norm.x < -bound) norm.x = -1;
				if (norm.x > -bound) norm.x = 0;
			}
			return norm;
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

Vector2 lines_intersect(Vector2 p11, Vector2 p12, Vector2 p21, Vector2 p22, bool deep_check = false) {
	float Z  = (p12.y-p11.y)*(p21.x-p22.x)-(p21.y-p22.y)*(p12.x-p11.x);
	float Ca = (p12.y-p11.y)*(p21.x-p11.x)-(p21.y-p11.y)*(p12.x-p11.x);
	float Cb = (p21.y-p11.y)*(p21.x-p22.x)-(p21.y-p22.y)*(p21.x-p11.x);
	
	if ((Z ==0) && (Ca == 0) && (Cb == 0)) {
		return Vector2(-1,-1);
	}
	if (Z == 0) {
		return Vector2(-1,-1);
	}
	
	float Ua = Ca/Z;
	float Ub = Cb/Z;
	if ((0 <= Ua) && (Ua <= 1) && (0 <= Ub) && (Ub <= 1)) {
		return Vector2(p11.x + (p12.x - p11.x) * Ub, p11.y + (p12.y - p11.y) * Ub);
	} else {
		if (deep_check) {
			return Vector2(p11.x + (p12.x - p11.x) * Ub, p11.y + (p12.y - p11.y) * Ub);
		}
		
	}
	return Vector2(-1,-1);
}

template <class T>
class Array {
	private:
		size_t count;
		std::map<size_t,T> data;
	public:
		Array() {
			this->count = 0;
		}
		~Array() {
			this->count = 0;
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
			this->data[this->count-1] = item;
			return this->count-1;
		}
		typename std::map<size_t,T>::iterator find(size_t id) {
			return this->data.find(id);
		}
		void del_item (size_t i) {
		}
		size_t size() const {
			return this->count;
		}
		void clear() {
			this->count = 0;
			this->data.clear();
		}
};


class Particle {
	private:
		PeasantID id;
		Section *ul;
		Section *ur;
		Section *dl;
		Section *dr;
		Vector2 position;
		Vector2 speed;
		Vector2 projected_position;
		Uint32 color;
		float width, height;
		float inv_mass;
		bool is_pinned;
		
		std::list<Section *> sections;
	public:
		Particle() {
			this->id = 0;
			this->ul = NULL;
			this->ur = NULL;
			this->dl = NULL;
			this->dr = NULL;
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
			this->ul = NULL;
			this->ur = NULL;
			this->dl = NULL;
			this->dr = NULL;
			this->position = position;
			this->speed = speed;
			this->projected_position = position;
			this->color = color;
			this->width = width;
			this->height = height;
			this->inv_mass = inv_mass;
			this->is_pinned = is_pinned;
		}
	public:
		void add_section_id(Section *sc) {
			this->sections.push_back(sc);	
		}
		void del_section_id(size_t id); // have to be forward-declared
		                                // and defined AFTER class Section.
		
		void clear_section_ids() {
			this->sections.clear();
		}
		std::list<Section *>::iterator get_section_ids_begin() {
			return this->sections.begin();
		}
		std::list<Section *>::iterator get_section_ids_end() {
			return this->sections.end();
		}
	public:
		void set_id(PeasantID value) {
			this->id = value;
		}
		void set_ul(Section * value) {
			this->ul = value;
		}
		void set_ur(Section * value) {
			this->ur = value;
		}
		void set_dl(Section * value) {
			this->dl = value;
		}
		void set_dr(Section * value) {
			this->dr = value;
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
		Section * get_ul(void) {
			return this->ul;
		}
		Section * get_ur(void) {
			return this->ur;
		}
		Section * get_dl(void) {
			return this->dl;
		}
		Section * get_dr(void) {
			return this->dr;
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
		
		Section *u;
		Section *d;
		Section *l;
		Section *r;
		
		std::list<Particle *> particles;
	public:
		void add_particle_id(Particle *pt) {
			this->particles.push_back(pt);
		}
		void del_particle_id(size_t id) {
			for (std::list<Particle *>::iterator it=this->get_particle_ids_begin(); it != this->get_particle_ids_end(); it++) {
				if ((size_t)(*it)->get_id() == id) {
					this->particles.erase(it);
					break;
				}
			}
		}
		
		void clear_particle_ids() {
			this->particles.clear();
		}
		std::list<Particle *>::iterator get_particle_ids_begin() {
			return this->particles.begin();
		}
		std::list<Particle *>::iterator get_particle_ids_end() {
			return this->particles.end();
		}
		
	public:
		Section() {
			this->id = 0;
			this->x = 0;
			this->y = 0;
			this->w = 0;
			this->h = 0;
			
			this->u = NULL;
			this->d = NULL;
			this->l = NULL;
			this->r = NULL;

		}
		Section(PeasantPos x
		       ,PeasantPos y
		       ,PeasantSize w = 0
		       ,PeasantSize h = 0) {
			this->id = 0;
			this->x = x;
			this->y = y;
			this->w = w;
			this->h = h;

			this->u = NULL;
			this->d = NULL;
			this->l = NULL;
			this->r = NULL;
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
		void set_u(Section * value) {
			this->u = value;
		}
		void set_d(Section * value) {
			this->d = value;
		}
		void set_l(Section * value) {
			this->l = value;
		}
		void set_r(Section * value) {
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
		Section * get_u(void) {
			return this->u;
		}
		Section * get_d(void) {
			return this->d;
		}
		Section * get_l(void) {
			return this->l;
		}
		Section * get_r(void) {
			return this->r;
		}

};

void Particle::del_section_id(size_t id) {
	for (std::list<Section *>::iterator it=this->get_section_ids_begin(); it != this->get_section_ids_end(); it++) {
		if ((size_t)(*it)->get_id() == id) {
			this->sections.erase(it);
			break;
		}
	}
}

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
		World(PeasantSize s_w, PeasantSize s_h) {
			this->s_w = s_w;
			this->s_h = s_h;
			this->data = new WorldData;
			Section *sc = this->add_section(Section(0,0,s_w,s_h));
			this->u = sc;
			this->d = sc;
			this->l = sc;
			this->r = sc;

		}		
	
	public:
		Section *add_section(Section sc) {
			sc.set_w(this->s_w);
			sc.set_h(this->s_h);

			size_t id = this->data->sections.add_item(sc);
			std::map<size_t,Section>::iterator it = this->data->sections.find((size_t)id);
			this->sections.push_back(&it->second);
			it->second.set_id((PeasantID)id);
			return &it->second;
		}
		Particle *add_particle(Particle pt) {
			std::map<size_t,Section>::iterator scit = this->data->sections.find(0);
			pt.set_ur(&scit->second);
			pt.set_ul(&scit->second);
			pt.set_dr(&scit->second);
			pt.set_dl(&scit->second);
			size_t id = this->data->particles.add_item(pt);
			std::map<size_t,Particle>::iterator it  = this->data->particles.find((size_t)id);
			this->particles.push_back(&it->second);
			it->second.set_id((PeasantID)id);
			return &it->second;
		}
	public:
		std::list<Section *>::iterator get_sections_begin() {
			return this->sections.begin();
		}
		std::list<Section *>::iterator get_sections_end() {
			return this->sections.end();
		}
		std::list<Particle *>::iterator get_particles_begin() {
			return this->particles.begin();
		}
		std::list<Particle *>::iterator get_particles_end() {
			return this->particles	.end();
		}
	public:
	
		bool move_particles(bool init=false) {
			for (std::list<Particle *>::iterator i=this->get_particles_begin(); i != this->get_particles_end(); i++) {
				Particle *pt = *i;
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				if (position == projected_position && init == false) {
					continue;
				}
		
				
				PeasantSize pt_width = pt->get_width();				
				PeasantSize pt_height = pt->get_height();


				Section *sc_u = this->get_u();
				Section *sc_d = this->get_d();
				Section *sc_l = this->get_l();
				Section *sc_r = this->get_r();
				
				PeasantPos section_max_x = sc_r->get_x();
				PeasantPos section_min_x = sc_l->get_x();
				PeasantPos section_min_y = sc_u->get_y();
				PeasantPos section_max_y = sc_d->get_y();
				

				Section *p_ur = pt->get_ur();
				Section *p_ul = pt->get_ul();
				Section *p_dr = pt->get_dr();
				Section *p_dl = pt->get_dl();
				
				PeasantPos p_section_ul_x = p_ul->get_x();
				PeasantPos p_section_ul_y = p_ul->get_y();
				PeasantPos p_section_dr_x = p_dr->get_x();
				PeasantPos p_section_dr_y = p_dr->get_y();
				
				if (position.x != projected_position.x || init == true) {
					while ((projected_position.x + pt_width/2 + this->s_w/2) > ((section_max_x+1) * this->s_w)) {
						if (sc_r->get_r() == NULL) {
							sc_r = gen_new_section(sc_r,MOVE_RIGHT);
							section_max_x = sc_r->get_x();
							this->set_r(sc_r);
						}

					}

					while ((projected_position.x - pt_width/2 - this->s_w/2) < ((section_min_x) * this->s_w)) {
						if (sc_l->get_l() == NULL) {
							sc_l = gen_new_section(sc_l,MOVE_LEFT);
							section_min_x = sc_l->get_x();
							this->set_l(sc_l);
						}
					}
					
					while ((projected_position.x + pt_width/2) > ((p_section_dr_x+1) * this->s_w)) {
						p_dr = p_dr->get_r();
						p_ur = p_ur->get_r();
						pt->set_dr(p_dr);
						pt->set_ur(p_ur);
						p_section_dr_x = p_dr->get_x();
					}
					while ((projected_position.x + pt_width/2) < ((p_section_dr_x) * this->s_w)) {
						p_dr = p_dr->get_l();
						p_ur = p_ur->get_l();
						pt->set_dr(p_dr);
						pt->set_ur(p_ur);
						p_section_dr_x = p_dr->get_x();
					}
					while ((projected_position.x - pt_width/2) < ((p_section_ul_x) * this->s_w)) {
						p_ul = p_ul->get_l();
						p_dl = p_dl->get_l();
						pt->set_ul(p_ul);
						pt->set_dl(p_dl);
						p_section_ul_x = p_ul->get_x();
					}
					while ((projected_position.x - pt_width/2) > ((p_section_ul_x+1) * this->s_w)) {
						p_ul = p_ul->get_r();
						p_dl = p_dl->get_r();
						pt->set_ul(p_ul);
						pt->set_dl(p_dl);
						p_section_ul_x = p_ul->get_x();
					}
				}
				if (position.y != projected_position.y || init == true) {
					while ((projected_position.y - pt_height/2 - this->s_h/2) < ((section_min_y) * this->s_h)) {
						if (sc_u->get_u() == NULL) {
							sc_u = gen_new_section(sc_u,MOVE_UP);
							section_min_y = sc_u->get_y();
							this->set_u(sc_u);
						}
					}

					while ((projected_position.y + pt_height/2 + this->s_h/2) > ((section_max_y+1) * this->s_h)) {
						if (sc_d->get_d() == NULL) {
							sc_d = gen_new_section(sc_d,MOVE_DOWN);
							section_max_y = sc_d->get_y();
							this->set_d(sc_d);
						}

					}
					while ((projected_position.y - pt_height/2) < ((p_section_ul_y) * this->s_h)) {
						p_ul = p_ul->get_u();
						p_ur = p_ur->get_u();
						pt->set_ul(p_ul);
						pt->set_ur(p_ur);
						p_section_ul_y = p_ul->get_y();
					}
					while ((projected_position.y - pt_height/2) > ((p_section_ul_y+1) * this->s_h)) {
						p_ul = p_ul->get_d();
						p_ur = p_ur->get_d();
						pt->set_ul(p_ul);
						pt->set_ur(p_ur);
						p_section_ul_y = p_ul->get_y();
					}
					while ((projected_position.y + pt_height/2) > ((p_section_dr_y+1) * this->s_h)) {
						p_dr = p_dr->get_d();
						p_dl = p_dl->get_d();
						pt->set_dr(p_dr);
						pt->set_dl(p_dl);
						p_section_dr_y = p_dr->get_y();
					}
					while ((projected_position.y + pt_height/2) < ((p_section_dr_y) * this->s_h)) {
						p_dr = p_dr->get_u();
						p_dl = p_dl->get_u();
						pt->set_dr(p_dr);
						pt->set_dl(p_dl);
						p_section_dr_y = p_dr->get_y();
					}
				}
				
				//printf("\n%d - %d\n%d - %d\n",p_ul->get_id(),p_ur->get_id(),p_dl->get_id(),p_dr->get_id());
				
				for (std::list<Section *>::iterator it=pt->get_section_ids_begin(); it != pt->get_section_ids_end(); it++) {
					(*it)->del_particle_id((size_t)pt->get_id());
				}
				
				pt->clear_section_ids();
				
				Section *sc_y,*sc_x;
				
				sc_y = p_ul;
				
				for (PeasantPos y = p_ul->get_y(); y <= p_dr->get_y(); y++) {
					sc_x = sc_y;
					for (PeasantPos x = p_ul->get_x(); x <= p_dr->get_x(); x++) {
						pt->add_section_id(sc_x);
						sc_x->add_particle_id(pt);
						sc_x = sc_x->get_r();
					}
					sc_y = sc_y->get_d();
				}
				
				pt->set_position(projected_position);
			}
			return true;
		}
		
		Section *gen_new_section(Section *current_section, Direction direction, bool gen = false) {
			PeasantPos x = current_section->get_x();
			PeasantPos y = current_section->get_y();
			Section sc(x,y);
			
			switch (direction) {
				case INVALID:
					return NULL;
					break;
				case MOVE_UP:
					if (current_section->get_u() != NULL) {
						printf("reject: u\n");
						return NULL;
					}
					sc.set_y(y-1);
					sc.set_d(current_section);
					break;
				case MOVE_DOWN:
					if (current_section->get_d() != NULL) {
						printf("reject: d\n");
						return NULL;
					}
					sc.set_y(y+1);
					sc.set_u(current_section);
					break;
				case MOVE_LEFT:
					if (current_section->get_l() != NULL) {
						printf("reject: l\n");
						return NULL;
					}
					sc.set_x(x-1);
					sc.set_r(current_section);
					break;
				case MOVE_RIGHT:
					if (current_section->get_r() != NULL) {
						printf("reject: r\n");
						return NULL;
					}
					sc.set_x(x+1);
					sc.set_l(current_section);
					break;
			}
			Section *newid = this->add_section(sc);

			Section *neigh;
			Section *sect;
			
					
			switch (direction) {
				case INVALID:
					return NULL;
					break;
				case MOVE_UP:
					current_section->set_u(newid);
					if (gen == false) {
						sect = newid;
						neigh = current_section->get_l();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_LEFT,true);
							neigh->set_u(sect);
							sect->set_d(neigh);
							neigh = neigh->get_l();
						}
						sect = newid;
						neigh = current_section->get_r();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_RIGHT,true);
							neigh->set_u(sect);
							sect->set_d(neigh);
							neigh = neigh->get_r();
						}
					}
					break;
				case MOVE_DOWN:
					current_section->set_d(newid);
					if (gen == false) {
						sect = newid;
						neigh = current_section->get_l();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_LEFT,true);
							neigh->set_d(sect);
							sect->set_u(neigh);
							neigh = neigh->get_l();
						}
						sect = newid;
						neigh = current_section->get_r();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_RIGHT,true);
							neigh->set_d(sect);
							sect->set_u(neigh);
							neigh = neigh->get_r();
						}
					}
					break;
				case MOVE_LEFT:
					current_section->set_l(newid);
					if (gen == false) {
						sect = newid;
						neigh = current_section->get_u();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_UP,true);
							neigh->set_l(sect);
							sect->set_r(neigh);
							neigh = neigh->get_u();
						}
						sect = newid;
						neigh = current_section->get_d();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_DOWN,true);
							neigh->set_l(sect);
							sect->set_r(neigh);
							neigh = neigh->get_d();
						}
					}
					break;
				case MOVE_RIGHT:
					current_section->set_r(newid);
					if (gen == false) {
						sect = newid;
						neigh = current_section->get_u();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_UP,true);
							neigh->set_r(sect);
							sect->set_l(neigh);
							neigh = neigh->get_u();
						}
						sect = newid;
						neigh = current_section->get_d();
						while (neigh != NULL) {
							sect = gen_new_section(sect,MOVE_DOWN,true);
							neigh->set_r(sect);
							sect->set_l(neigh);
							neigh = neigh->get_d();
						}
					}
					break;
			}
			return newid;
		}
		
	public:
		void set_u(Section * value) {
			this->u = value;
		}
		void set_d(Section * value) {
			this->d = value;
		}
		void set_l(Section * value) {
			this->l = value;
		}
		void set_r(Section * value) {
			this->r = value;
		}

	public:
		Section * get_u(void) {
			return this->u;
		}
		Section * get_d(void) {
			return this->d;
		}
		Section * get_l(void) {
			return this->l;
		}
		Section * get_r(void) {
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
		void render(Section *sc, bool conn) {
			SDL_Rect rect;
			rect.w = 10;
			rect.h = 20;
			if (sc->get_u() != NULL) {
				rect.y = sc->get_y() * sc->get_h() + yoffset - sc->get_h()/2 + sc->get_h()/2 - 10;
				rect.x = sc->get_x() * sc->get_w() + xoffset+sc->get_w()/4;
				SDL_FillRect(this->screen,&rect,SDL_MapRGB(screen->format,0xFF, 0x00, 0x00));
			}
			if (sc->get_d() != NULL) {
				rect.y = sc->get_y() * sc->get_h() + yoffset + sc->get_h() - 10;
				rect.x = sc->get_x() * sc->get_w() + xoffset+sc->get_w()/4*3;
				SDL_FillRect(this->screen,&rect,SDL_MapRGB(screen->format,0x00, 0xFF, 0x00));
			}
			rect.w = 20;
			rect.h = 10;
			if (sc->get_r() != NULL) {
				rect.y = sc->get_y() * sc->get_h() + yoffset + sc->get_h()/4;
				rect.x = sc->get_x() * sc->get_w() + xoffset + sc->get_w() - 10;
				SDL_FillRect(this->screen,&rect,SDL_MapRGB(screen->format,0x00, 0x00, 0xFF));
			}
			if (sc->get_l() != NULL) {
				rect.y = sc->get_y() * sc->get_h() + yoffset + sc->get_h()/4*3;
				rect.x = sc->get_x() * sc->get_w() + xoffset - 10;
				SDL_FillRect(this->screen,&rect,SDL_MapRGB(screen->format,0x00, 0xFF, 0xFF));
			}
			
			rect.x = sc->get_x() * sc->get_w() + xoffset;
			rect.y = sc->get_y() * sc->get_h() + yoffset;
			rect.w = sc->get_w();
			rect.h = sc->get_h();
			SDL_Surface *message = NULL;
			char number[1024];
			snprintf(number,1023,"%ld",sc->get_id());
			message = TTF_RenderText_Solid( font, number, text_color );	
			rect.x += rect.w/2-(Sint16)strlen(number);
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
			if (rect.x + rect.w < 0 || rect.y + rect.h < 0) return;
			SDL_FillRect(this->screen,&rect,pt->get_color());
		}
		void redraw() {
			SDL_FillRect(this->screen,&this->screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));
		}
		void flip() {
			SDL_Flip(this->screen);
		}
};

class PhysEngineST {
	private:
		World *world;
		Vector2 gravity;
		float dt;
	public:
		PhysEngineST(World *world) {
			this->world = world;
			this->gravity = Vector2(0,0);
			this->dt = 0.01;
		}
	public:
		void compute() {
			for (std::list<Particle *>::iterator i=this->world->get_particles_begin(); i != this->world->get_particles_end(); i++) {
				Particle *pt = *i;
				if (pt->get_is_pinned()) continue;
				Vector2 speed = pt->get_speed();
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				speed += gravity * dt;
				projected_position = position + speed;
				
				pt->set_speed(speed);
				pt->set_projected_position(projected_position);
			}
			for (std::list<Section *>::iterator i=this->world->get_sections_begin(); i != this->world->get_sections_end(); i++) {
				for (std::list<Particle *>::iterator ptit = (*i)->get_particle_ids_begin(); ptit != (*i)->get_particle_ids_end(); ptit++) {
					ptit++;
					for (std::list<Particle *>::iterator ptnt = ptit--; ptnt != (*i)->get_particle_ids_end(); ptnt++) {
						Particle *pti = *ptit;
						Particle *ptn = *ptnt;
						
						if (pti->get_is_pinned() && ptn->get_is_pinned()) continue;
						
						Vector2 p1 = pti->get_projected_position();
						Vector2 p2 = ptn->get_projected_position();

						float p1_width = pti->get_width();
						float p1_height = pti->get_height();
						float p2_width = ptn->get_width();
						float p2_height = ptn->get_height();

						Vector2 p1_ul(p1.x - p1_width/2,p1.y - p1_height/2);
						Vector2 p1_ur(p1.x + p1_width/2,p1.y - p1_height/2);
						Vector2 p1_dl(p1.x - p1_width/2,p1.y + p1_height/2);
						Vector2 p1_dr(p1.x + p1_width/2,p1.y + p1_height/2);
						
						Vector2 p1_u(p1.x,p1.y + p1_height/2);
						Vector2 p1_d(p1.x,p1.y - p1_height/2);
						Vector2 p1_l(p1.x - p1_width/2,p1.y);
						Vector2 p1_r(p1.x + p1_width/2,p1.y);
						
						Vector2 p2_ul(p2.x - p2_width/2,p2.y - p2_height/2);
						Vector2 p2_ur(p2.x + p2_width/2,p2.y - p2_height/2);
						Vector2 p2_dl(p2.x - p2_width/2,p2.y + p2_height/2);
						Vector2 p2_dr(p2.x + p2_width/2,p2.y + p2_height/2);
						
						Vector2 p2_u(p2.x,p2.y + p2_height/2);
						Vector2 p2_d(p2.x,p2.y - p2_height/2);
						Vector2 p2_l(p2.x - p2_width/2,p2.y);
						Vector2 p2_r(p2.x + p2_width/2,p2.y);
						
					
						
						float pen_vert = 0.0f;
						float pen_horiz = 0.0f;
																		
						bool u_inv = false;
						bool d_inv = false;
						bool r_inv = false;
						bool l_inv = false;
										
						Vector2 up_to_bottom(-1,-1);
						Vector2 bottom_to_up(-1,-1);
						Vector2 right_to_left(-1,-1);
						Vector2 left_to_right(-1,-1);
						
						Vector2 pdiff(0,0);
						Vector2 pdiff_norm(0,0);						
						
						float amount = 1;
						
						if (p2_l.x >= p1_l.x && p2_r.x <= p1_r.x) {
							up_to_bottom = lines_intersect(p1_dl,p1_dr,p2_d,p2_u);
							if (up_to_bottom != Vector2(-1,-1)) pdiff += Vector2(0,amount);
						} else if (p2_l.x < p1_l.x && p2_r.x > p1_r.x) {
							up_to_bottom = lines_intersect(p1_ul,p1_ur,p2_d,p2_u);
							if (up_to_bottom != Vector2(-1,-1)) pdiff += Vector2(0,-amount);
							u_inv = true;
						} else if (p2_r.x > p1_l.x && p2_l.x < p1_l.x && (p2_r.x - p1_l.x) > (p1_u.y - p2_d.y)) {
							up_to_bottom = lines_intersect(p1_dl,p1_dr,p2_ur,p2_dr);
							if (up_to_bottom != Vector2(-1,-1)) pdiff += Vector2(0,amount);
						} else if (p2_l.x < p1_r.x && p2_r.x > p1_r.x  && (p1_r.x - p2_l.x) > (p1_u.y - p2_d.y)) {
							up_to_bottom = lines_intersect(p1_dl,p1_dr,p2_ul,p2_dl);
							if (up_to_bottom != Vector2(-1,-1)) pdiff += Vector2(0,amount);
						}
						
						if (p2_l.x >= p1_l.x && p2_r.x <= p1_r.x) {
							bottom_to_up = lines_intersect(p1_ul,p1_ur,p2_u,p2_d);
							if (bottom_to_up != Vector2(-1,-1)) pdiff += Vector2(0,-amount);
						} else if (p2_l.x < p1_l.x && p2_r.x > p1_r.x) {
							bottom_to_up = lines_intersect(p1_dl,p1_dr,p2_u,p2_d);
							if (bottom_to_up != Vector2(-1,-1)) pdiff += Vector2(0,amount);
							d_inv = true;
						} else if (p2_r.x > p1_l.x && p2_l.x < p1_l.x && (p2_r.x - p1_l.x) > (p2_u.y - p1_d.y)) {
							bottom_to_up = lines_intersect(p1_ul,p1_ur,p2_ur,p2_dr);
							if (bottom_to_up != Vector2(-1,-1)) pdiff += Vector2(0,-amount);
						} else if (p2_l.x < p1_r.x && p2_r.x > p1_r.x && (p1_r.x - p2_l.x) > (p2_u.y - p1_d.y)) {
							bottom_to_up = lines_intersect(p1_ul,p1_ur,p2_ul,p2_dl);
							if (bottom_to_up != Vector2(-1,-1)) pdiff += Vector2(0,-amount);
						}
						
						if (p1_u.y >= p2_u.y && p1_d.y <= p2_d.y) {
							right_to_left = lines_intersect(p1_ur,p1_dr,p2_r,p2_l);
							if (right_to_left != Vector2(-1,-1)) pdiff += Vector2(amount,0);
						} else if (p1_u.y < p2_u.y && p1_d.y > p2_d.y) {
							right_to_left = lines_intersect(p1_ul,p1_dl,p2_r,p2_l);
							if (right_to_left != Vector2(-1,-1)) pdiff += Vector2(-amount,0);
							r_inv = true;
						} else if (p1_u.y > p2_u.y && p1_d.y < p2_u.y && (p2_u.y - p1_d.y) > (p1_r.x - p2_l.x)) {
							right_to_left = lines_intersect(p1_ur,p1_dr,p2_dr,p2_dl);
							if (right_to_left != Vector2(-1,-1)) pdiff += Vector2(amount,0);
						} else if (p1_d.y < p2_d.y && p1_u.y > p2_d.y && (p1_u.y - p2_d.y) > (p1_r.x - p2_l.x)) {
							right_to_left = lines_intersect(p1_ur,p1_dr,p2_ur,p2_ul);
							if (right_to_left != Vector2(-1,-1)) pdiff += Vector2(amount,0);
						}
						
												
						if (p1_u.y >= p2_u.y && p1_d.y <= p2_d.y) {
							left_to_right = lines_intersect(p1_ul,p1_dl,p2_r,p2_l);
							if (left_to_right != Vector2(-1,-1)) pdiff += Vector2(-amount,0);
						} else if (p1_u.y < p2_u.y && p1_d.y > p2_d.y) {
							left_to_right = lines_intersect(p1_ur,p1_dr,p2_r,p2_l);
							if (left_to_right != Vector2(-1,-1)) pdiff += Vector2(amount,0);
							l_inv = true;
						} else if (p1_u.y > p2_u.y && p1_d.y < p2_u.y && (p2_u.y - p1_d.y) > (p2_r.x - p1_l.x)) {
							left_to_right = lines_intersect(p1_ul,p1_dl,p2_dr,p2_dl);
							if (left_to_right != Vector2(-1,-1)) pdiff += Vector2(-amount,0);
						} else if (p1_d.y < p2_d.y && p1_u.y > p2_d.y && (p1_u.y - p2_d.y) > (p2_r.x - p1_l.x)) {
							left_to_right = lines_intersect(p1_ul,p1_dl,p2_ur,p2_ul);
							if (left_to_right != Vector2(-1,-1)) pdiff += Vector2(-amount,0);
						}
						


						
						if (up_to_bottom == Vector2(-1,-1)
						 && bottom_to_up == Vector2(-1,-1)
						 && left_to_right == Vector2(-1,-1)
						 && right_to_left == Vector2(-1,-1)) {
							continue;
						}
						
						//printf("%f %f\n",pdiff.x,pdiff.y);
						//SDL_Delay(100);
						
						if (up_to_bottom != Vector2(-1,-1)) {
							if (u_inv == false) {
								pen_vert = up_to_bottom.y - p2_d.y;
							} else {
								pen_vert = p2_u.y - up_to_bottom.y;
							}
						} else if (bottom_to_up != Vector2(-1,-1)) {
							if (d_inv == false) {
								pen_vert = p2_u.y - bottom_to_up.y;
							} else {
								pen_vert = bottom_to_up.y - p2_d.y;
							}
						}
						
						if (left_to_right != Vector2(-1,-1)) {
							if (l_inv == false) {
								pen_horiz = p2_r.x - left_to_right.x;
							} else {
								pen_horiz = left_to_right.x - p2_l.x;
							}
						} else if (right_to_left != Vector2(-1,-1)) {
							if (r_inv == false) {
								pen_horiz = right_to_left.x - p2_l.x;
							} else {
								pen_horiz = p2_r.x - right_to_left.x;
							}
						}


						float w1 = pti->get_inv_mass();
						float w2 = ptn->get_inv_mass();
						float width = (p1_width/2 + p2_width/2)/2;
						float height = (p1_height/2 + p2_height/2)/2;
						Vector2 dp1(0,0);
						Vector2 dp2(0,0);
						
						pdiff_norm = pdiff.normalize();
						
						dp1 = pdiff_norm * Vector2(pen_horiz,pen_vert) * w1 * 0.5;
						dp2 = pdiff_norm * Vector2(pen_horiz,pen_vert) * w2 * 0.5;
						
						
						p1 -= dp1;
						p2 += dp2;
						
						pti->set_projected_position(p1);						
						ptn->set_projected_position(p2);
					}
				}
			}
			for (std::list<Particle *>::iterator i=this->world->get_particles_begin(); i != this->world->get_particles_end(); i++) {
				Particle *pt = *i;
				if (pt->get_is_pinned()) continue;
				Vector2 speed = pt->get_speed();
				Vector2 position = pt->get_position();
				Vector2 projected_position = pt->get_projected_position();
				
				speed = projected_position - position;
				
				pt->set_speed(speed);
			}
		}
	
};

int main(int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1000,700,32,SDL_SWSURFACE);
	TTF_Init();

	World world(100,100);
	PhysEngineST phys(&world);
	SDLRenderer render(screen,0,0);
	
	
	for (int n=0; n < 10; n++){
		for (int i=0; i < 10; i++) {
			world.add_particle(Particle(Vector2(150+i*55,150+n*55),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0xFF,0x00),50,50,0.1,false));
		}
	}
	
	
	
	world.add_particle(Particle(Vector2(500,10),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),1000,20,0,true));
	world.add_particle(Particle(Vector2(500,690),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),1000,20,0,true));
	world.add_particle(Particle(Vector2(10,350),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),20,700,0,true));
	world.add_particle(Particle(Vector2(990,350),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),20,700,0,true));
	
	/*	
	world.add_particle(Particle(Vector2(100,100),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(100,300),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(100,500),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(300,100),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(300,300),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(300,500),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(500,100),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(500,300),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	world.add_particle(Particle(Vector2(500,500),Vector2(0,0),SDL_MapRGB(screen->format,0x00,0x00,0xFF),100,100,1,false));
	*/
	
		
	Particle *pt = NULL;
	pt = world.add_particle(Particle(Vector2(150,350),Vector2(0,0),SDL_MapRGB(screen->format,0xFF,0x00,0x00),50,50,1,false));
	world.move_particles(true);

	bool quit = false; 
	SDL_Event event;
	Uint8 *keystates = SDL_GetKeyState(NULL);
	Uint8 old_keystates[SDLK_LAST];
	memcpy(old_keystates,keystates,SDLK_LAST * sizeof(Uint8));
	while (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) quit = true;


		float speed = 5;
		if (keystates[SDLK_UP]) {
			pt->set_speed(Vector2(0,-speed));
		}
		if (keystates[SDLK_DOWN]) {
			pt->set_speed(Vector2(0,speed));
		}
		if (keystates[SDLK_LEFT]) {
			pt->set_speed(Vector2(-speed,0));
		}
		if (keystates[SDLK_RIGHT]) {
			pt->set_speed(Vector2(speed,0));
		}
		if (keystates[SDLK_z]) {
			pt->set_speed(Vector2(0,0));
		}


		Uint32 ticks = SDL_GetTicks();
		phys.compute();
		world.move_particles();
		printf("%d\n",SDL_GetTicks()-ticks);

		memcpy(old_keystates,keystates,SDLK_LAST * sizeof(Uint8));


		render.redraw();
		for (std::list<Section *>::iterator i=world.get_sections_begin(); i != world.get_sections_end(); i++) {
			render.render(*i);
		}
		for (std::list<Section *>::iterator i=world.get_sections_begin(); i != world.get_sections_end(); i++) {
			render.render(*i,true);
		}
		for (std::list<Particle *>::iterator i=world.get_particles_begin(); i != world.get_particles_end(); i++) {
			render.render(*i);
		}
		
		render.flip();
		SDL_Delay(0);
	}
	
	return 0;  
}
