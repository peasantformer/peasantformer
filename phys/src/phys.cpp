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
			if (position.x < 0 || position.y < 0) return;
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
		Particle *operator[](size_t i) {
			return this->members[i];
		}
	public:
		size_t add_member(Particle *pt) {
			return this->members.add_item(pt);
		}
		void del_member(size_t i) {
			this->members.del_item(i);
		}
		size_t size() {
			return this->members.size();
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
	public:
		Section get_sec(size_t i) {
			return this->sections[i];
		}
		size_t size_sec() {
			return this->sections.size();
		}
		
		Particle get_obj(size_t i) {
			return this->objects[i];
		}
		size_t size_obj() {
			return this->objects.size();
		}
		void add_obj(Particle pt) {
			int id;
			id = this->objects.add_item(pt);
			this->place_obj(id);
		}
		void del_obj(int i) {
			this->objects.del_item(i);
		}
		void rmove_obj(size_t id) {
			for (size_t i=0; i < this->objects[id].size_rev(); i++) {
				this->sections[objects[id].get_rev(i).section].del_member(objects[id].get_rev(i).index);
			}
			this->objects[id].clear_rev();
		}
		void place_obj(size_t id) {
			size_t max_x = (this->objects[id].position.x + this->objects[id].width/2) / this->sec_width;
			size_t min_x = (this->objects[id].position.x - this->objects[id].width/2) / this->sec_width;
			size_t max_y = (this->objects[id].position.y + this->objects[id].height/2) / this->sec_height;
			size_t min_y = (this->objects[id].position.y - this->objects[id].height/2) / this->sec_height;
			
			rmove_obj(id);
			size_t s,cnt;
			bool was_asigned = false;
			for (size_t i=min_x; i <= max_x; i++) {
				if (i >= this->width || y < 0) continue;
				for (size_t n=min_y; n <= max_y; n++) {
					if (n >= this->height || n < 0) continue;
					cnt = height * i + n;
					s = this->sections[cnt].add_member(&objects[id]);
					this->objects[id].add_rev(cnt,s);
					was_asigned = true;
				}
			}
			if (was_asigned == false) {
				del_obj(id);
			}
		}
};



int main(int argc, char **argv) {
//	int level_width = 800;
//	int level_height = 600;
//	int section_width = 100;
//	int section_height = 100;

	
//	pt1.add_rev(0,2);

//	

	Level lvl(0,0,200,200,100,100);
	lvl.add_obj(Particle(Vector2(50,100), Vector2(0,0), 50, 50, 1, 1, false));
	lvl.place_obj(0);
//	Array<Particle> pts;
	
//	pts.add_item(Particle(Vector2(150,150), Vector2(0,0), 100, 100, 1, 1, false));
//	printf("|||||||||| %d\n",(int)pts[0].position.x);


//	lvl.add_obj();
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
