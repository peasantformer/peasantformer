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
		int count;
		int alloc;
		T *data;
	public:
		Array() {
			this->count = 0;
			this->alloc = 0;
			this->data = NULL;
		}
		~Array() {
			this->count = 0;
			this->alloc = 0;
			free(this->data);
		}
	public:
		T operator[](int i) {
			return this->data[i];
		}
	public:
		int add_item(T item) {
			this->count++;
			if (this->count > this->alloc) {
				this->alloc++;
				this->data = (T*)realloc(this->data,this->alloc * sizeof(T));
			}
			this->data[this->count-1] = item;
			return this->count-1;
		}
		void del_item (int i) {
			if (this->count == 0) return;
			this->count--;
			if (this->count == i) return;
			this->data[i] = this->data[this->count];
		}
		int size() {
			return this->count;
		}
		void clear() {
			this->count = 0;
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
	public:
		Array<Particle *> members;
		int x,y;
	public:
		Section(int x, int y) {
			this->x = x;
			this->y = y;
		}
		~Section() {
			printf("decon\n");
		}
	public:
		Particle *operator[](int i) {
			return this->members[i];
		}
	public:
		int add_member(Particle *pt) {
			return this->members.add_item(pt);
		}
		void del_member(int i) {
			this->members.del_item(i);
		}
		int size() {
			return this->members.size();
		}
};

class Level {
	public:
		Array<Section> sections;
		Array<Particle> objects;
		
		int x,y;
		int width,height;
		int sec_width, sec_height;
	public:
		Level(int x, int y, int u_w, int u_h, int s_w, int s_h) {
			this->x = x;
			this->y = y;
			this->width = u_w/s_w;
			this->height = u_h/s_h;
			if (this->width == 0) this->width = 1;
			if (this->height == 0) this->height = 1;
			this->sec_width = s_w;
			this->sec_height = s_h;
			
			for (int x=0; x < this->width; x++) {
				for (int y=0; y < this->height; y++) {
					printf("%d sec %d %d\n",sections.size(),x,y);
					Section *sec = (Section *) malloc(sizeof(Section));
					*sec = Section(x,y);
					this->sections.add_item(*sec);
				}
			}
			printf("%d\n",this->sections.size());
		}
		~Level() {
			printf("level decon\n");
		}
	public:
		void add_obj(Particle pt) {
			int id;
			id = this->objects.add_item(pt);
			this->place_obj(id);
		}
		void del_obj(int i) {
			this->objects.del_item(i);
		}
		void place_obj(int id) {
			int max_x = (this->objects[id].position.x + this->objects[id].width/2) / this->sec_width;
			int min_x = (this->objects[id].position.x - this->objects[id].width/2) / this->sec_width;
			int max_y = (this->objects[id].position.y + this->objects[id].height/2) / this->sec_height;
			int min_y = (this->objects[id].position.y - this->objects[id].height/2) / this->sec_height;
			
			int s,cnt;
			for (int i=min_x; i <= max_x; i++) {
				if (i >= this->width || y < 0) continue;
				for (int n=min_y; n <= max_y; n++) {
					if (n >= this->height || n < 0) continue;
					cnt = height * i + n;
					
//					this->sections[0].add_member(&objects[id]);
//					printf("%f\n",this->objects[0].position.x);
//					printf("%d\n",this->sections[0].size());
//					printf("%d %d %p\n",cnt,id,&this->objects[id]);
				}
			}
		}
};

int main(int argc, char **argv) {
	int level_width = 800;
	int level_height = 600;
	int section_width = 100;
	int section_height = 100;

	Level lvl(0,0,200,200,100,100);
	lvl.add_obj(Particle(Vector2(50,150), Vector2(0,0), 50, 50, 1, 1, false));
//	printf("%d\n",lvl.sections[0].size());
//	printf("%d\n",lvl.sections.members);
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
