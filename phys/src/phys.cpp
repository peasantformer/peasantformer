#include <math.h>
#include <vector>
#include <string.h>
#include <stdlib.h>
#include "SDL/SDL.h"

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


struct RevSectIndex {
	int section;
	int index;
};

class Particle {
	public:
		Vector2 position;
		Vector2 speed;
		float width;
		float height;
		float inv_mass;
		float bounceness;
		bool pinned;
		
		RevSectIndex *rev_sections;
		int rev_count;
		int rev_alloc;
	public:
		Particle() {
			this->position = Vector2(0,0);
			this->speed = Vector2(0,0);
			this->width = 1;
			this->height = 1;
			this->inv_mass = 1;
			this->bounceness = 1;
			this->pinned = false;
			this->rev_sections = NULL;
			this->rev_count = 0;
			this->rev_alloc = 0;
		}
		Particle(Vector2 p, Vector2 s, float w, float h, float m, float b, bool pinned) {
			this->position = p;
			this->speed = s;
			this->width = w;
			this->height = h;
			this->inv_mass = m;
			this->bounceness = b;
			this->pinned = pinned;
			this->rev_sections = NULL;
			this->rev_count = 0;
			this->rev_alloc = 0;
		}
	public:
		void add_rev(int section, int index) {
			this->rev_count++;
			
			if (this->rev_count > this->rev_alloc) {
				this->rev_alloc++;
				this->rev_sections = (RevSectIndex *)realloc(this->rev_sections, this->rev_alloc * sizeof(RevSectIndex));
			}
			RevSectIndex rsi;
			rsi.section = section;
			rsi.index = index;
			this->rev_sections[this->rev_count-1] = rsi;
		}
		void del_rev(int n) {
			if (this->rev_count == 0) return;
			int i=0;
			
			for (int z=0; z < this->rev_count; z++) {
				if (this->rev_sections[z].section == n) {
					i = z;
					break;
				}
			}
			this->rev_count--;
			if (this->rev_count == i) return;
			this->rev_sections[i] = this->rev_sections[this->rev_count];
		}
		int size_rev() {
			return this->rev_count;
		}
		void draw() {
			if (position.x < 0 || position.y < 0) return;
		}
		
};

class Section {
	private:
		int count;
		int allocated;
		Particle **members;
	public:
		int x;
		int y;
	public:
		Section(int x, int y) {
			this->x = x;
			this->y = y;
			
			this->count = 0;
			this->allocated = 0;
			this->members = NULL;
		}
		~Section() {
			free(this->members);
			this->members = NULL;
		}
	public:
		Particle *operator[](int i) {
			return this->members[i];
		}
	public:
		int add(Particle *particle) {
			this->count++;
			if (this->count > this->allocated) {
				this->allocated++;
				this->members = (Particle **)realloc(this->members, this->allocated * sizeof(Particle *));
			}

			this->members[this->count-1] = particle;
			return this->count-1;
		}
		void del(int i) {
			if (this->count == 0) return;
			this->count--;
			if (this->count == i) return;
			this->members[i] = this->members[this->count];
		}
		int size() {
			return this->count;
		}
		
};


class Level {
	private:
		int sec_count;
		Section *sections;
		
		int pt_count;
		int pt_alloc;
		Particle *particles;
	public:
		int x,y;
		int width,height;
		int section_width;
		int section_height;
	public:
		Level(int x, int y, int un_width, int un_height, int s_w, int s_h) {
			this->x = x;
			this->y = y;
			this->width = un_width/s_w;
			this->height = un_height/s_h;
			if (this->width == 0) this->width = 1;
			if (this->height == 0) this->height = 1;
			this->section_width = s_w;
			this->section_height = s_h;
			this->sec_count = this->width * this->height;
			this->sections = (Section *)malloc(this->sec_count * sizeof(Section));
			this->pt_count = 0;
			this->pt_alloc = 0;
			this->particles = NULL;

			int cnt=0;
			for (int x=0; x < this->width; x++) {
				for (int y=0; y < this->height; y++) {
					this->sections[cnt++] = Section(x,y);
				}
			}
		}
		~Level() {
			free(this->sections);
			free(this->particles);
			this->sections = NULL;
			this->particles = NULL;
		}
	public:
		void add_obj(Particle pt) {
			int x = (int)pt.position.x/(int)section_width;
			int y = (int)pt.position.y/(int)section_height;
			
			if (x >= this->width || y >= this->height) return;

			this->pt_count++;

			if (this->pt_count > this->pt_alloc) {
				this->pt_alloc++;
				this->particles = (Particle *)realloc(this->particles, this->pt_alloc * sizeof (Particle));
			}
			
			this->particles[this->pt_count-1] = pt;

			int cnt = height * x + y;
			int s;
			
			
//			s = this->sections[cnt].add(&this->particles[this->pt_count-1]);
//			this->particles[this->pt_count-1].add_rev(cnt,s);
			
						
		}
};


int main(int argc, char **argv) {
	int level_width = 800;
	int level_height = 600;
	int section_width = 100;
	int section_height = 100;

	Level lvl(0,0,200,800,100,100);
	lvl.add_obj(Particle(Vector2(190,190), Vector2(0,0), 10, 10, 1, 1, false));
//	s.add(Particle(Vector2(150,150), Vector2(0,0), 10, 10, 1, 1, false));
/*
	World world;
	world.add(0,0,800,600,100,100);
	
	world[0]->add_obj(Particle(Vector2(	150,150), Vector2(0,0), 10, 10, 1, 1, false));
	
	printf("%d\n",world[0]->sec_size());

	return 0;
	for (int i=0, x=0; i < level_width; i += section_width, x++) {
		for (int n=0, y=0; n < level_height; n += section_height, y++) {
//			world[0]->add(x,y,i,n);
		}
	}
	
//	for (int i=0; i < world
*/
	
	return 0;
}
