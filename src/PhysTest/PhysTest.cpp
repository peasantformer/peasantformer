#include "PhysTest.h"

class Point {
	public:
		Vector2f pos;
		Vector2f proj_pos;
		Vector2f speed;
		float inv_mass;
		bool was_computed;
		bool pinned;
		bool was_touched;

		Point() : 
			pos(0,0), 
			proj_pos(0,0), 
			speed(0,0), 
			inv_mass(1), 
			was_computed(false),
			pinned(false)
		{}
		Point(Vector2f pos, float inv_mass, bool pinned) :
			pos(pos),
			proj_pos(pos),
			speed(0,0),
			inv_mass(inv_mass),
			was_computed(false),
			pinned(pinned)
		{}

		void draw() {
			glLoadIdentity();
			glTranslatef(pos.x, pos.y, 0 );
			
			glColor3f(1.0,1.0,1.0);
			//if (pos.x > 1000 || pos.x < -1000) exit(0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
			glEnd();
			glLoadIdentity();
		}
};

class Joint {
	public:
		Point *p1;
		Point *p2;
		float stiffness;
		float length1;
		bool upper;
		Joint *prev;
		Joint *next;

		Joint() : 
			p1(NULL), 
			p2(NULL), 
			stiffness(0), 
			length1(0), 
			upper(true),
			prev(NULL),
			next(NULL)
		{}
		Joint(Point *p1, Point *p2, float length1, float stiffness, bool upperm) :
			p1(p1),
			p2(p2),
			stiffness(stiffness),
			length1(length1),
			upper(upper),
			prev(NULL),
			next(NULL)
		{

		}
	
		void draw() {
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);
			glLineWidth(50);
			glBegin(GL_LINES);
				glVertex2f(p1->pos.x,p1->pos.y);
				glVertex2f(p2->pos.x,p2->pos.y);
 			glEnd();
			glLoadIdentity();
		}
		void apply() {
		/*
		
				

			if (p2->proj_pos != p2->pos) {

				if (p3->proj_pos == p3->pos) {
				
					Vector2f cat1 = p2->proj_pos - p1->proj_pos;
					cat1.x = -cat1.x;
					float hyp1 = sqrt(cat1.x * cat1.x + cat1.y * cat1.y);
					float sine1 = cat1.y / hyp1;
					float cosine1 = cat1.x / hyp1;
					Vector2f res1(-length1 * cosine1,length1 * sine1);

					p2->proj_pos += (p1->proj_pos + res1) - p2->proj_pos;
					
					Vector2f cat2 = p2->proj_pos - p3->proj_pos;
					cat2.x = -cat2.x;
					float hyp2 = sqrt(cat2.x * cat2.x + cat2.y * cat2.y);
					float sine2 = cat2.y / hyp2;
					float cosine2 = cat2.x / hyp2;
					Vector2f res2(-length2 * cosine2,length2 * sine2);
					
					p2->proj_pos += (p3->proj_pos + res2) - p2->proj_pos;
					p1->porj_pos = p3->poj
				} 
				if (p1->proj_pos == p1->pos) {
					Vector2f cat2 = p2->proj_pos - p3->proj_pos;
					cat2.x = -cat2.x;
					float hyp2 = sqrt(cat2.x * cat2.x + cat2.y * cat2.y);
					float sine2 = cat2.y / hyp2;
					float cosine2 = cat2.x / hyp2;
					Vector2f res2(-length2 * cosine2,length2 * sine2);
					
					//p2->proj_pos += (p3->proj_pos + res2) - p2->proj_pos;
					
					
					Vector2f cat1 = p2->proj_pos - p1->proj_pos;
					cat1.x = -cat1.x;
					float hyp1 = sqrt(cat1.x * cat1.x + cat1.y * cat1.y);
					float sine1 = cat1.y / hyp1;
					float cosine1 = cat1.x / hyp1;
					Vector2f res1(-length1 * cosine1,length1 * sine1);

					p2->proj_pos += (p1->proj_pos + res1) - p2->proj_pos;
				}
			}
			

			if (p3->proj_pos != p3->pos) {

				Vector2f cat = p3->proj_pos - p2->proj_pos;
				
				cat.x = -cat.x;
				
				float hyp = sqrt(cat.x * cat.x + cat.y * cat.y);

				float sine = cat.y / hyp;
				float cosine = cat.x / hyp;

				Vector2f res(
					-length1 * cosine,
					length1 * sine
				);
				
				p3->proj_pos = p2->proj_pos + res; 
			}
			if (p3->proj_pos == p3->pos) {
				if (p1->proj_pos != p1->pos) {

					
					Vector2f cat = p1->proj_pos - p2->proj_pos;
					
					cat.x = -cat.x;
					
					float hyp = sqrt(cat.x * cat.x + cat.y * cat.y);

					float sine = cat.y / hyp;
					float cosine = cat.x / hyp;

					Vector2f res(-length1 * cosine,length1 * sine);
					
					p1->proj_pos = p2->proj_pos + res;

				}
			}
		*/
		
			if (p1->proj_pos == p1->pos && p2->proj_pos != p2->pos) {
				Vector2f cat2 = p2->proj_pos - p1->proj_pos;
				cat2.x = -cat2.x;
				float hyp2 = sqrt(cat2.x * cat2.x + cat2.y * cat2.y);
				float sine2 = cat2.y / hyp2;
				float cosine2 = cat2.x / hyp2;
				Vector2f res2(-length1 * cosine2,length1 * sine2);
				p2->proj_pos += (p1->proj_pos + res2) - p2->proj_pos;
					if (this->next != NULL && (p1->proj_pos != p1->pos || this->next->p2->proj_pos != this->next->p2->pos)) {
						this->next->p2->proj_pos = p2->proj_pos + res2;
					}
				p2->was_computed = true;
			}

			
			if (p2->proj_pos == p2->pos && p1->proj_pos != p1->pos) {
				Vector2f cat2 = p1->proj_pos - p2->proj_pos;
				cat2.x = -cat2.x;
				float hyp2 = sqrt(cat2.x * cat2.x + cat2.y * cat2.y);
				float sine2 = cat2.y / hyp2;
				float cosine2 = cat2.x / hyp2;
				Vector2f res2(-length1 * cosine2,length1 * sine2);
				p1->proj_pos += (p2->proj_pos + res2) - p1->proj_pos;
					if (this->prev != NULL  && (p2->proj_pos != p2->pos || this->prev->p1->proj_pos != this->prev->p1->pos)) {
						this->prev->p1->proj_pos = p1->proj_pos + res2;
					}
				p1->was_computed = true;
			}


			
			if (p2->proj_pos != p2->pos && p1->proj_pos != p1->pos) {
				if (p1->was_computed == true) {
					Vector2f cat2 = p2->proj_pos - p1->proj_pos;
					cat2.x = -cat2.x;
					float hyp2 = sqrt(cat2.x * cat2.x + cat2.y * cat2.y);
					float sine2 = cat2.y / hyp2;
					float cosine2 = cat2.x / hyp2;
					Vector2f res2(-length1 * cosine2,length1 * sine2);
					p2->proj_pos += (p1->proj_pos + res2) - p2->proj_pos;
					
					p2->was_computed = true;

				} else {
					Vector2f cat2 = p1->proj_pos - p2->proj_pos;
					cat2.x = -cat2.x;
					float hyp2 = sqrt(cat2.x * cat2.x + cat2.y * cat2.y);
					float sine2 = cat2.y / hyp2;
					float cosine2 = cat2.x / hyp2;
					Vector2f res2(-length1 * cosine2,length1 * sine2);
					p1->proj_pos += (p2->proj_pos + res2) - p1->proj_pos;

					p1->was_computed = true;
				}
			}

		}
	
};

enum ObjectType {
	O_INVALID,
	O_STICK,
	O_PLAIN,
	O_LAST
};

class Plain;
class Stick;

class Object {
	public:
		ObjectType type;
	
		virtual ~Object() {};
	
		virtual void draw() = 0;
		virtual void compute() = 0;
		virtual void compute(Stick *) = 0;
		virtual void compute(Plain *) = 0;
		virtual void project(Vector2f, float) = 0;
		virtual void update(float) = 0;
};

class Stick : public Object {
	public:
		bool pinned;
		Point p1;
		Point p2;
		Point p3;

		Joint holder;
		Joint holder2;
	
		Stick() : pinned(false) {}
		Stick(Vector2f pos, float length) :
			pinned(false),
			p1(Vector2f(pos.x-length,pos.y),1,false),
			p2(Vector2f(pos.x,pos.y),1,true),
			p3(Vector2f(pos.x+length,pos.y),1,false),
			holder(&this->p1, &this->p2, length, 0.5,false),
			holder2(&this->p2, &this->p3, length, 0.5, false)
		{
			holder2.prev = &holder;
			holder.next = &holder2;
			this->type = O_STICK;
		}
	
	
		Stick& operator=(const Stick &rhs) {
			if (this == &rhs)
				return *this;
			this->p1 = rhs.p1;
			this->p2 = rhs.p2;
			this->p3 = rhs.p3;
			this->holder = rhs.holder;
			this->holder2 = rhs.holder2;

			this->holder.p1 = &this->p1;
			this->holder.p2 = &this->p2;
			
			this->holder2.p1 = &this->p2;
			this->holder2.p2 = &this->p3;
			
			this->holder.next = &this->holder2;
			this->holder2.prev = &this->holder;

			return *this;
		}
	
		void draw() {
			this->p1.draw();
			this->p2.draw();
			this->p3.draw();

			this->holder.draw();
			this->holder2.draw();
			
			glLoadIdentity();
			
			glColor3f(0.0,1.0,0.0);
			glLineWidth(1);
			
			glBegin(GL_LINE_LOOP);
				glVertex2f(this->p1.pos.x-15,this->p1.pos.y-15);
				glVertex2f(this->p1.pos.x-15,this->p1.pos.y+15);
				glVertex2f(this->p2.pos.x+15,this->p2.pos.y+15);
				glVertex2f(this->p2.pos.x+15,this->p2.pos.y-15);
			glEnd();

			glLoadIdentity();
		}
		void compute() {
			this->p1.was_computed = false;
			this->p2.was_computed = false;
			this->p3.was_computed = false;
			this->p1.was_touched = false;
			this->p2.was_touched = false;
			this->p3.was_touched = false;
			this->holder.apply();
			this->holder2.apply();
		}
		void compute(Stick *obj) {
		}
		void compute(Plain *obj) {
		}
		void project_pt(Point *p, Vector2f ext, float dt) {
			if (p->pinned == true) return;
			p->speed = p->speed + ext * dt * p->inv_mass;
			p->proj_pos = p->pos + p->speed * dt;
		}
		void project(Vector2f ext,float dt) {
			project_pt(&p1,ext,dt);
			project_pt(&p2,ext,dt);
			project_pt(&p3,ext,dt);
		}
		void update_pt(Point *p, float dt) {
			if (p->pinned == true) return;
			p->speed = (p->proj_pos - p->pos) / dt;
			p->pos = p->proj_pos;
		}
		void update(float dt) {
			update_pt(&p1, dt);
			update_pt(&p2, dt);
			update_pt(&p3, dt);
		}
};

class Plain : public Object {
	public:
		bool pinned;
		Vector2f pos;
		float width;
		float height;
	public:
		Plain() : pinned(true) {}
		Plain(Vector2f pos, float width, float height) :
			pinned(true),
			pos(pos),
			width(width),
			height(height)
		{
			this->type = O_PLAIN;
		}
	public:
		void draw() {
			glLoadIdentity();
			glTranslatef(pos.x, pos.y, 0 );
			glColor3f(0.5,0.7,0.7);
			glBegin(GL_QUADS);
				glVertex2f(-width,-height);
				glVertex2f(width,-height);
				glVertex2f(width,height);
				glVertex2f(-width,height);
			glEnd();
			glLoadIdentity();
		}
		void compute() {
		}
		void compute(Stick *obj) {
		}
		void compute(Plain *obj) {
		}
		void project(Vector2f ext, float dt) {
		}
		void update(float dt) {
		}
};

class World {
	public:
		Array<Stick> sticks;
		Array<Plain> plains;
		std::list<Object *> objects;
		
		Vector2f gravity;
		float dt;

		World() : gravity(0,10), dt(0.005) {}

		void add_stick(Vector2f pos, float length) {
			size_t num;
			num = this->sticks.add_item(Stick(pos,length));
			this->objects.push_back(this->sticks.get_find_ref(num));
		}
		void add_plain(Vector2f pos, float width, float height) {
			size_t num;
			num = this->plains.add_item(Plain(pos,width,height));
			this->objects.push_back(this->plains.get_find_ref(num));
		}

		void draw() {
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				(*it)->draw();
			}
		}
		void iterate() {
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				(*it)->project(gravity,dt);
			}
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				for (std::list<Object *>::iterator ti = it; ti != this->objects.end(); ti++) {
					if (ti == it) (*it)->compute(); // sellf
					if ((*ti)->type == O_PLAIN) (*it)->compute((Plain *)*ti);
					if ((*ti)->type == O_STICK) (*it)->compute((Stick *)*ti);
				}
			}
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				(*it)->update(dt);
			}
		}
};

void test() {
	SDL_Event event;
    bool quit = false;
    
    
    Vector2f init(300,300);
    Vector2f vec(-200,0);
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		  
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);
			glLineWidth(50);
			glBegin(GL_LINES);
				glVertex2f(init.x,init.y);
				glVertex2f(init.x + vec.x,init.y+vec.y);
			glEnd();
			glLoadIdentity();
			glColor3f(0.0,0.0,1.0);
			
			float res_x = (vec.x) * cos(45) + (0) * sin(45);
			float res_y = -(vec.x) * sin(45) + (0) * cos(45);
			vec.x = res_x;
			vec.y = res_y;
			
			
			glBegin(GL_LINES);
				glVertex2f(init.x,init.y);
				glVertex2f(init.x + vec.x,init.y+vec.y);
			glEnd();
			glLoadIdentity();
		
		SDL_GL_SwapBuffers();
    
    while(quit == false) {
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			quit = true;
	}

	
	
	exit(0);
}

int main(int argc, char **argv) {

	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(800,600,32, SDL_OPENGL);
	SDL_WM_SetCaption(",д,Я,Й,Б,",NULL);

	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,800, 600,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	World world;
	
	
	world.add_stick(Vector2f(200,200),60);
	world.add_plain(Vector2f(300,600),600,100);
	
	//test();
	
	SDL_Event event;
    bool quit = false;
    
    while(quit == false) {
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			quit = true;
		
		world.iterate();
		
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		
		world.draw();		
		
		SDL_GL_SwapBuffers();
		
	}
	
	
	return 0;
}
