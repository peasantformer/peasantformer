
#include <cmath>
#include <vector>
#include "SDL/SDL.h"
#include "SDL/SDL_thread.h"

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
		Array<size_t> collides;
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
		
		void add_col(size_t id) {
			this->collides.add_item(id);
		}
		void clear_col() {
			this->collides.clear();
		}
		bool test_col(size_t id) {
			for (size_t i=0; i < this->collides.size(); i++) {
				if (this->collides[i] == id) return true;
			}
			return false;
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




class World {
	public:
		Array<Section *> sections;
		size_t x,y;
	public:
		World() {
			this->x = 0;
			this->y = 0;
		}
		World(size_t x, size_t y) {
			this->x = x;
			this->y = y;
		}
	public:
		Section *get_sec(size_t i) {
			return this->sections[i];
		}
		size_t add_sec(Section *sec) {
			return this->sections.add_item(sec);
		}
		void del_sec(size_t i) {
			this->sections.del_item(i);
		}
		size_t size() {
			return this->sections.size();
		}
		void clear() {
			this->sections.clear();
		}
};

class Level {
	public:
		Array<Section> sections;
		Array<Particle> objects;
		
		size_t x,y;
		size_t width,height;
		int sec_width, sec_height;
		World *world;
	public:
		Level(World *world, size_t x, size_t y, size_t u_w, size_t u_h, size_t s_w, size_t s_h) {
			this->world = world;
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
					this->world->add_sec(&this->sections[this->sections.size()-1]);
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
					this->objects[id].id = id;
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




class Computor;

struct thread_data {
	Computor *obj;
	size_t id;
	size_t begin;
	size_t end;
};

class Computor {
	public:
		std::vector<SDL_Thread *> threads;
		std::vector<bool> thread_over;
		std::vector<bool> active;
		size_t no_of_threads;
		SDL_sem *got_it;
		SDL_sem *ready;
		size_t slice;
		World *world;
	public:
		Computor(World *world, int no) {
			this->world = world;
			this->slice = this->world->size() / no;
			if (this->world->size() % no) {	
				slice += no / (this->world->size() % no);
			}	
//			printf("%d\n",slice);
			this->no_of_threads = no;
			this->got_it = SDL_CreateSemaphore(0);
			this->ready = SDL_CreateSemaphore(0);
			this->thread_gen();
		}
		~Computor() {
			for (size_t i=0; i < this->no_of_threads; i++) {
				SDL_KillThread(threads[i]);
			}
			SDL_DestroySemaphore(this->got_it);
			SDL_DestroySemaphore(this->ready);
		}
	public:
		void thread_gen() {
			active.clear();
			threads.clear();
			for (size_t i=0; i < this->no_of_threads; i++) {
				thread_data td;
				td.obj = this;
				td.id = i;
				td.begin = i * this->slice;
				td.end = i * this->slice + this->slice - 1;
				if (td.end >= this->world->size()) td.end = this->world->size() - 1;
				active.push_back(false);
				thread_over.push_back(true);
				threads.push_back(SDL_CreateThread(this->thread,&td));
				SDL_SemWait(this->ready);
			}
		}
		void iterate() {
			for (size_t i=0; i < this->no_of_threads; i++){
				this->active[i] = true;
				this->thread_over[i] = false;
				SDL_SemWait(this->got_it);
			}
			for (size_t i=0; i < this->no_of_threads; i++){
				while (this->thread_over[i] == false)
					SDL_Delay(1);
			}
		}
	public:
		void i_got_it(size_t i) {
			this->active[i] = false;
			SDL_SemPost(this->got_it);	
		}
		void i_ready() {
			SDL_SemPost(this->ready);
		}
		void i_came(size_t i) {
			this->thread_over[i] = true;
		}
		bool is_active(size_t i) {
			if (i < 0 || i >= this->active.size()) // Q: why do i need this ?
				return false;                  // A: to handle impossible case.
			return this->active[i];
		}
		Section *get_data(size_t i) {
			return this->world->get_sec(i);	
		}
	public:
		static int thread(void *data) {
			thread_data *parent_touch = (thread_data *)data;
			Computor *parent = parent_touch->obj;
			size_t id = parent_touch->id;
			size_t begin = parent_touch->begin;
			size_t end = parent_touch->end;
			parent->i_ready();
			while (true) {
				if (parent->is_active(id)) {
					parent->i_got_it(id);
					for (size_t i=begin; i <= end; i++) {
//						size_t c = parent->get_data(i);
//						printf("thread #%d - %d\n",id,i);
//						factorial(c);
					}
					parent->i_came(id);
				}
				SDL_Delay(1);
			}
			return 0;
		}
	
};



int main(int argc, char **argv) {


	SDL_Init(SDL_INIT_EVERYTHING);
	screen = SDL_SetVideoMode(1100,600,32,SDL_SWSURFACE);

	World world;
	Level lvl(&world,0,0,1000,500,20,20);
	
	Computor cmpt(&world, 1);


	for (size_t i=0; i < 1000/20; i++) {
		lvl.add_obj(Particle(Vector2 (10+i*20,10), Vector2 (0,0), 20, 20, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
		lvl.add_obj(Particle(Vector2 (10+i*20,450), Vector2 (0,0), 20, 20, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
	}
	for (size_t i=0; i < 600/20; i++) {
		lvl.add_obj(Particle(Vector2 (10,10+i*20), Vector2 (0,0), 20, 20, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
		lvl.add_obj(Particle(Vector2 (990,10+i*20), Vector2 (0,0), 20, 20, 1, 1, true,SDL_MapRGB(screen->format,0xFF,0x00,0x00)));
	}



	for (size_t i=0; i < 100; i++) {
		for (size_t n=0; n < 50; n++) {
			lvl.add_obj(Particle(Vector2 (50+i*6,50+n*6), Vector2 (0,0), 5, 5, 1, 1, false,SDL_MapRGB(screen->format,0x00,0x00,0xFF)));
		}
	}

	
	lvl.rebuild_obj_links();
	
	bool quit = false; 
	SDL_Event event;
	while (quit == false) {	
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT) quit = true;

		int ticks = SDL_GetTicks();

//		phys_engine(&lvl,4);
		cmpt.iterate();

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
