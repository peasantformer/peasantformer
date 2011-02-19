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
		float x,y;
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
		int width;
		int height;
	public:
		Section(int x, int y, int width, int height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			
			this->count = 0;
			this->allocated = 0;
			this->members = NULL;
		}
		~Section() {
			for (int i=0; i < this->allocated; i++) {
				free(this->members[i]);
				this->members[i] = NULL;
			}
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
//				this->members[this->count-1] = (Particle *)malloc(sizeof (Particle));
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
		int pt_allocated;
		Particle *particles;
		
	public:
		int x;
		int y;
		int width;
		int height;
		int abs_width;
		int abs_height;
		int section_width;
		int section_height;
	public:
		Level(int x, int y, int width, int height, int s_w, int s_h) {
			this->x = x;
			this->y = y;
			this->abs_width = width;
			this->abs_height = height;

			this->width = (width/s_w) * s_w;
			this->height = (height/s_h) * s_h;
			
			this->section_width = s_w;
			this->section_height = s_h;

			this->sec_count = (width/s_w) * (height/s_h);
			
			this->sections = (Section *)malloc(this->sec_count * sizeof(Section));
			
			int cnt=0;
			for (int x=0; x < width/s_w; x++) {
				for (int y=0; y < height/s_h; y++) {
					this->sections[cnt++] = Section(x,y,s_w,s_h);
				}
			}

		}
		~Level() {
			free(this->sections);
			this->sections = NULL;
		}
	public:
		Section *operator[](int i) {
			return &this->sections[i];
		}
	public:
		void add_obj(Particle pt) {
			
		}
		void del_obj(int sec, int i) {

		}
		void move_obj(int sec, int i) {
			
		}
		int sec_size() {
			return this->sec_count;
		}
		
};

class World {
	private:
		int count;
		int allocated;
		Level *levels;
	public:
		World() {
			this->count = 0;
			this->allocated = 0;
			this->levels = NULL;
		}
		~World() {
			free(this->levels);
			this->levels = NULL;
		}
	public:
		Level *operator[](int i) {
			return &this->levels[i];
		}
	public:
		void add(int x, int y, int width, int height, int s_x, int s_y) {
			this->count++;
			if (this->count > this->allocated) {
				this->allocated++;
				this->levels = (Level *)realloc(this->levels, this->allocated * sizeof(Level));
			}
			this->levels[this->count-1] = Level(x,y,width,height,s_x,s_y);
		}
		void del(int i) {
			if (this->count == 0) return;
			this->count--;
			if (this->count == i) return;
			this->levels[i] = this->levels[this->count];
		}
		int size() {
			return this->count;
		}
		
};


int main(int argc, char **argv) {
	int level_width = 800;
	int level_height = 600;
	int section_width = 100;
	int section_height = 100;

	World world;
	world.add(0,0,800,600,100,100);
	
	printf("%d\n",world[0]->sec_size());

	return 0;
	for (int i=0, x=0; i < level_width; i += section_width, x++) {
		for (int n=0, y=0; n < level_height; n += section_height, y++) {
//			world[0]->add(x,y,i,n);
		}
	}
	
//	for (int i=0; i < world

	
	return 0;
}
