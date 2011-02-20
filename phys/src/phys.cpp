#include <math.h>
#include <list>
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

template <class T>
class Array {
	private:
		int size;
		int alloc;
		T *data;
	public:
		Array() {
			this->size = 0;
			this->alloc = 0;
			this->data = NULL;
		}
		~Array() {
			this->size = 0;
			this->alloc = 0;
			free(this->data);
		}
	public:
		T operator[](int i) {
			return this->data[i];
		}
	public:
		void add_item(T item) {
			this->size++;
			if (this->size > this->alloc) {
				this->alloc++;
				this->data = (T*)realloc(this->data,sizeof(T));
			}
			this->data[this->size-1] = item;
		}
		void del_item (int i) {
			if (this->size == 0) return;
			this->count--;
			if (this->count == i) return;
			this->data[i] = this->data[this->count];
		}
		void clear() {
			this->size = 0;
			this->alloc = 0;
			free(this->data);
			this->data = NULL;	
		}
};

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
		
		Array<RevSectIndex> rev_sections;
	public:
		Particle() {
			this->position = Vector2(0,0);
			this->speed = Vector2(0,0);
			this->width = 1;
			this->height = 1;
			this->inv_mass = 1;
			this->bounceness = 1;
			this->pinned = false;

		}
		Particle(Vector2 p, Vector2 s, float w, float h, float m, float b, bool pinned) {
			this->position = p;
			this->speed = s;
			this->width = w;
			this->height = h;
			this->inv_mass = m;
			this->bounceness = b;
			this->pinned = pinned;

		}

	public:
		void add_rev(int section, int index) {
			RevSectIndex rsi;
			rsi.section = section;
			rsi.index = index;
			this->rev_sections.add_item(rsi);
		}
		
		void clear_rev() {
			this->rev_sections.clear();
		}
		
		void draw() {
			if (position.x < 0 || position.y < 0) return;
		}
		
};

class Section {
	private:
		std::list<Particle *> members;
	public:
		int x,y;
	public:
		Section(int x, int y) {
			this->x = x;
			this->y = y;
		}
	public:
		Particle *operator[](int i) {
			std::list<Particle *>::iterator it;
			it = this->members.begin();
			return *it;
		}
		
};

int main(int argc, char **argv) {
	int level_width = 800;
	int level_height = 600;
	int section_width = 100;
	int section_height = 100;

//	Level lvl(0,0,200,200,100,100);
//	lvl.add_obj(Particle(Vector2(150,150), Vector2(0,0), 100, 100, 1, 1, false));
//	lvl.add_obj(Particle(Vector2(150,150), Vector2(0,0), 100, 100, 1, 1, false));	
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
