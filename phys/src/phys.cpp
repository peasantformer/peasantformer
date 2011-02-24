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


struct RevSectIndex {
	size_t section;
	size_t index;
	
};


class Particle {
	public:
		size_t id;
		Vector2 position;
		Vector2 speed;
		Vector2 p;
		Uint32 color;
		float width;
		float height;
		float inv_mass;
		float bounceness;
		bool pinned;
		
		Array<RevSectIndex> rev_sections;
	public:
		Particle() {
			this->id = 0;
			this->position = Vector2(0,0);
			this->speed = Vector2(0,0);
			this->p = Vector2(0,0);
			this->width = 1;
			this->height = 1;
			this->inv_mass = 1;
			this->bounceness = 1;
			this->pinned = false;
			this->color = SDL_MapRGB(screen->format,0xFF,0xFF,0xFF);

		}
		Particle(Vector2 p, Vector2 s, float w, float h, float m, float b, bool pinned, Uint32 c = SDL_MapRGB(screen->format,0xFF,0xFF,0xFF)) {
			this->id = 0;
			this->position = p;
			this->p = p;
			this->speed = s;
			this->width = w;
			this->height = h;
			this->inv_mass = m;
			this->bounceness = b;
			this->pinned = pinned;
			this->color = c;
		}

	public:
		RevSectIndex get_rev(size_t i) {
			return this->rev_sections[i];
		}
		void add_rev(size_t section, size_t index) {
			RevSectIndex rsi;
			rsi.section = section;
			rsi.index = index;
			this->rev_sections.add_item(rsi);
		}
		
		void clear_rev() {
			this->rev_sections.clear();
		}
		
		size_t size_rev() {
			return this->rev_sections.size();
		}
		
		void draw() {
//			if (position.x < 0 || position.y < 0) return;
			SDL_Rect rect;
			rect.x = this->position.x - this->width/2;
			rect.y = this->position.y - this->height/2;
			rect.w = this->width;
			rect.h = this->height;
			SDL_FillRect(screen,&rect,color);
		}
};


class Section {
	public:
		Array<Particle *> members;
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
		Particle *get_member(size_t i) {
			return this->members[i];
		}
		size_t add_member(Particle *pt) {
			return this->members.add_item(pt);
		}
		void del_member(size_t i) {
			this->members.del_item(i);
		}
		size_t size() {
			return this->members.size();
		}
		void clear() {
			this->members.clear();
		}
};

class Level {
	public:
		Array<Section> sections;
		Array<Particle> objects;
		
		size_t x,y;
		size_t width,height;
		int sec_width, sec_height;
	public:
		Level(size_t x, size_t y, size_t u_w, size_t u_h, size_t s_w, size_t s_h) {
			this->x = x;
			this->y = y;
			this->width = u_w/s_w;
			this->height = u_h/s_h;
			if (this->width == 0) this->width = 1;
			if (this->height == 0) this->height = 1;
			this->sec_width = s_w;
			this->sec_height = s_h;
			
			for (size_t x=0; x < this->width; x++) {
				for (size_t y=0; y < this->height; y++) {
					this->sections.add_item(Section(x,y));
				}
			}
		}
		
		Section *get_sec(size_t i) {
			return &this->sections[i];
		}
		size_t size_sec() {
			return this->sections.size();
		}
		
		Particle *get_obj(size_t i) {
			return &this->objects[i];
		}
		size_t size_obj() {
			return this->objects.size();
		}
		
		void add_obj(Particle pt) {
			int id;
			id = this->objects.add_item(pt);
			this->objects[id].id = id;
		}
		void del_obj(int i) {
			this->objects.del_item(i);
		}
			
		void rebuild_obj_links() {
			for (size_t id=0; id < this->objects.size(); id++) {
				for (size_t i=0; i < objects[id].size_rev(); i++) {
					RevSectIndex rev = objects[id].get_rev(i);
					this->sections[rev.section].del_member(rev.index);
				}
			}
			
			for (size_t i=0; i < this->objects.size(); i++) {
				this->place_obj(i);
			}
		}	
		void place_obj(size_t id) {
			if (this->objects[id].position.x < 0) return;
			if (this->objects[id].position.y < 0) return;
			size_t max_x = (this->objects[id].position.x + this->objects[id].width/2) / this->sec_width;
			size_t min_x = (this->objects[id].position.x - this->objects[id].width/2) / this->sec_width;
			size_t max_y = (this->objects[id].position.y + this->objects[id].height/2) / this->sec_height;
			size_t min_y = (this->objects[id].position.y - this->objects[id].height/2) / this->sec_height;
			
			size_t s,cnt;
			bool was_placed = false;

			this->objects[id].clear_rev();
			for (size_t i=min_x; i <= max_x; i++) {
				if (i >= this->width || y < 0) continue;
				for (size_t n=min_y; n <= max_y; n++) {
					if (n >= this->height || n < 0) continue;
					
					cnt = height * i + n;
					s = this->sections[cnt].add_member(&objects[id]);
					
					this->objects[id].add_rev(cnt,s);
					was_placed = true;
				}
			}
			if (was_placed == false) {
				del_obj(id);
			}

		}
		void draw_sections() {
			SDL_Rect rect;
			Uint32 color;
			for (size_t i=0; i < sections.size(); i++) {
				rect.x = sections[i].x * sec_width;
				rect.y = sections[i].y * sec_height;
				rect.w = sec_width;
				rect.h = sec_height;
				switch (i%3) {
					case 0:
						color = SDL_MapRGB(screen->format,0xFF,0xFF,0xFF);
						break;
					case 1:
						color = SDL_MapRGB(screen->format,0xaa,0xaa,0xaa);
						break;
					case 2:
						color = SDL_MapRGB(screen->format,0xaa,0xff	,0xaa);
						break;
				}
				SDL_FillRect(screen,&rect,color);
			}		
		}
};


void phys_engine(Level *lvl) {
	Vector2 gravity(0,9.8);
//	Vector2 gravity(0,0);
	float dt = 0.01;

	for (size_t i=0; i < lvl->size_obj(); i++) {
		if (lvl->get_obj(i)->pinned  == true) continue;
		lvl->get_obj(i)->speed += gravity * dt;
	}
	
	for (size_t i=0; i < lvl->size_obj(); i++) {
		if (lvl->get_obj(i)->pinned  == true) continue;
		lvl->get_obj(i)->p = lvl->get_obj(i)->position + lvl->get_obj(i)->speed * dt;
	}
	
	for (size_t z=0; z < lvl->size_sec(); z++) {
		Section *sec = lvl->get_sec(z);
		for (size_t i=0; i < sec->size(); i++) {
			for (size_t n=i+1; n < sec->size(); n++) {
			
				if (sec->get_member(n)->pinned == true) continue;
				
				Vector2 p1 = sec->get_member(i)->position;
				Vector2 p2 = sec->get_member(n)->position;
				
				float r1 = sec->get_member(i)->width/2;
				float r2 = sec->get_member(n)->width/2;
				if ((p2-p1).square_length() < (r1+r2)*(r1+r2)) {
					Vector2 pendir = (p2-p1).normalize();
					float pendep = (r1+r2) - (p2-p1).length();
					sec->get_member(i)->p -= pendir * pendep * 0.5;
					sec->get_member(n)->p += pendir * pendep * 0.5;
				}

			}
		}
	}
	
	for (size_t i=0; i < lvl->size_obj(); i++) {
		if (lvl->get_obj(i)->pinned  == true) continue;
		lvl->get_obj(i)->speed = (lvl->get_obj(i)->p - lvl->get_obj(i)->position) / dt;
		lvl->get_obj(i)->position = lvl->get_obj(i)->p;
	}
}

int main(int argc, char **argv) {


	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1100,600,32,SDL_SWSURFACE);
	

	Level lvl(0,0,1000,500,10,10);
/*
	lvl.add_obj(Particle(Vector2 (100,100), Vector2 (0,10), 50, 50, 1, 1, true	,SDL_MapRGB(screen->format,0x00,0x00,0xFF)));
	
	for (int i=0; i < 100; i++) {
		lvl.add_obj(Particle(Vector2 (50+i*10,400), Vector2 (0,-20), 5, 5, 1, 1, false,SDL_MapRGB(screen->format,0xAA,0x00,0x00)));
	}
*/	

	for (size_t i=0; i < 1000/5; i++) {
		lvl.add_obj(Particle(Vector2 (10+i*5,10), Vector2 (0,0), 5, 5, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
		lvl.add_obj(Particle(Vector2 (10+i*5,490), Vector2 (0,0), 5, 5, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
	}
	for (size_t i=0; i < 600/5; i++) {
		lvl.add_obj(Particle(Vector2 (10,10+i*5), Vector2 (0,0), 5, 5, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
		lvl.add_obj(Particle(Vector2 (990,10+i*5), Vector2 (0,0), 5, 5, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
	}

	for (size_t i=0; i < 100; i++) {
		for (size_t n=0; n < 50; n++) {
			lvl.add_obj(Particle(Vector2 (20+i*9,20+n*9), Vector2 (0,0), 5, 5, 1, 1, false,SDL_MapRGB(screen->format,0x00,0x00,0xFF)));
		}
	}
		

	
	lvl.rebuild_obj_links();
	
	bool quit = false; 
	SDL_Event event;
	while (quit == false) {	
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) quit = true;

		int ticks = SDL_GetTicks();

		phys_engine(&lvl);

		lvl.rebuild_obj_links();

		printf("%d @ %d\n",SDL_GetTicks()-ticks,(int)lvl.size_obj());		

		SDL_FillRect(screen,&screen->clip_rect,SDL_MapRGB(screen->format,0x00,0x00,0x00));		
		
		lvl.draw_sections();
		
		for (size_t i=0; i < lvl.size_obj(); i++) {
			lvl.get_obj(i)->draw();
		}

		SDL_Flip(screen);
	}
	SDL_Quit();
	return 0;
}
