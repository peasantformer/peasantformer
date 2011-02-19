#include <math.h>
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



class Particle {
	public:
		Vector2 position;
		Vector2 speed;
		float width;
		float height;
		float inv_mass;
		float bounceness;
		bool pinned;
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
		void draw() {
			if (position.x < 0 || position.y < 0) return;
		}
		
};

enum MemberType {
	FULL,
	PARTIAL
};

struct Member {
	Particle particle;
	MemberType type;
};

class Section {
	private:
		int count;
		int allocated;
		Member *members;
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
			free(this->members);
			this->members = NULL;
		}
	public:
		Member operator[](int i) {
			return this->members[i];
		}
	public:
		void add(Particle particle, MemberType type) {
			this->count++;
			if (this->count > this->allocated) {
				this->allocated++;
				this->members = (Member *)realloc(this->members, this->allocated * sizeof(Member));
			}
			Member member;
			member.particle = particle;
			member.type = type;
			this->members[this->count-1] = member;
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
		int count;
		int allocated;
		Section *sections;
	public:
		int x;
		int y;
		int width;
		int height;
	public:
		Level(int x, int y, int width, int height) {
			this->x = x;
			this->y = y;
			this->width = width;
			this->height = height;
			
			this->count = 0;
			this->allocated = 0;
			this->sections = NULL;
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
		void add(int x, int y, int width, int height) {
			this->count++;
			if (this->count > this->allocated) {
				this->allocated++;
				this->sections = (Section *)realloc(this->sections, this->allocated * sizeof(Section));
			}
			this->sections[this->count-1] = Section(x,y,width,height);
		}
		void del(int i) {
			if (this->count == 0) return;
			this->count--;
			if (this->count == i) return;
			this->sections[i] = this->sections[this->count];
		}
		int size() {
			return this->count;
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
		void add(int x, int y, int width, int height) {
			this->count++;
			if (this->count > this->allocated) {
				this->allocated++;
				this->levels = (Level *)realloc(this->levels, this->allocated * sizeof(Level));
			}
			this->levels[this->count-1] = Level(x,y,width,height);
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
	world.add(0,0,800,600);

	return 0;
	for (int i=0, x=0; i < level_width; i += section_width, x++) {
		for (int n=0, y=0; n < level_height; n += section_height, y++) {
			world[0]->add(x,y,i,n);
		}
	}

	
	return 0;
}
