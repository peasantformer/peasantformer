#include "PhysTest.h"

class Point {
	public:
		Vector2f pos;
		Vector2f prev_pos;
		Vector2f V;
		Vector2f F;
		Vector2f U;
		Vector2f C;
		float mass;
		float angle;
		float inertia;
		bool pinned;
		bool was_computed;
		Point *next;
		Point *prev;
		int index;
		bool refference;
	public:
		Point() :
			pos(0,0),
			V(0,0),
			F(0,0),
			U(0,0),
			C(0,0),
			mass(0),
			angle(0),
			inertia(0),
			pinned(false),
			was_computed(false),
			refference(false)
		{}
		Point(Vector2f pos, float mass, float angle, bool pinned) :
			pos(pos),
			prev_pos(pos),
			V(0,0),
			F(0,0),
			U(0,0),
			C(0,0),
			mass(mass),
			angle(angle),
			pinned(pinned),
			was_computed(false),
			refference(false)
		{
			inertia = mass * 50 * 50;
		}
	public:
		void place(bool refference = false) {
			if (refference) {
				pos = 
					next->pos +
					next->next->pos +
					next->next->next->pos +
					next->next->next->next->pos;
				this->refference = true;
			} else {
				this->angle = prev->angle - angle;
				pos = rotate(pos,angle);
				prev_pos = pos;
			}

		}
		void draw() {
		}
		void project(Vector2f ext, float dt) {
			if (this->pinned) return;
			if (this->refference) {
				pos = 
					next->pos +
					next->next->pos +
					next->next->next->pos +
					next->next->next->next->pos;
				return;
			}
			this->V += ext * dt + C * dt;
			this->prev_pos = this->pos;
			this->pos += this->V * dt;

		}
		void correct(Vector2f ext, float dt) {
			if (this->pinned || this->refference) return;
			this->V += F / mass * dt + U * dt;
			this->F = Vector2f(0,0);
			this->U = Vector2f(0,0);	
		}
};

float tmp = 0;

Vector2f R1,R2;
class Joint {
	public:
		Point *p1;
		Point *p2;
		float length;
		float angle;
	public:
		Joint() {
			this->p1 = NULL;
			this->p2 = NULL;
			this->length = 0;
			this->angle = 0;
		}
		Joint(Point *p1, Point *p2, float length, float angle) {
			this->p1 = p1;
			this->p2 = p2;
			this->length = length;
			this->angle = angle;
		}
	public:
		void draw() {
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);
			glLineWidth(10);
			glBegin(GL_LINES);
				glVertex2f(p1->pos.x,p1->pos.y);
				glVertex2f(p2->pos.x,p2->pos.y);
			glEnd();
			glLoadIdentity();
		}
	public:
		void apply(Vector2f ext, float dt, int direction) {
			Point *a,*b;
			a = p1->next;
			for (a = p1->next; a != p1; a = a->next) {
				if (a->pinned) break;
			}
			b=a->next;
			for (int i=0; i < 4; i++) {
				float adiff = length - a->pos.length();
				float bdiff = length - b->pos.length();
				//bool amoved = a->pos != a->prev_pos;
				//bool bmoved = b->pos != b->prev_pos;
				bool amoved = !a->pinned;
				bool bmoved = !b->pinned;
				if (amoved && bmoved) {
					Vector2f arealpos = 
						a->prev->prev->prev->pos +
						a->prev->prev->pos +
						a->prev->pos;
					Vector2f brealpos =
						b->prev->prev->prev->pos +
						b->prev->prev->pos +
						b->prev->pos;
					a->F = a->pos.normalize() * adiff;
					b->F = b->pos.normalize() * bdiff;
					//a->F = a->pos.normalize() * adiff;
					//b->F = b->pos.normalize() * bdiff;
				} else if (amoved && !bmoved) {
					

					Vector2f realpos = 
						a->prev->prev->prev->pos +
						a->prev->prev->pos +
						a->prev->pos;

					Vector2f diff = (realpos + a->pos);
					//a->F = diff * -length;
					a->F = a->pos.normalize() * adiff;

				} else if (!amoved && bmoved) {
					b->F = b->pos.normalize() * bdiff;
				}

				a = a->next;
				b = b->next;
			}
			a = p1;
			b = p1->prev;
			Vector2f first = p1->pos;
			Vector2f second = p1->next->pos;
			Vector2f third = p1->next->next->pos;
			Vector2f fourth = p1->next->next->next->pos;
			//printf("%f %f\n",(first + second).length(),second.length());

		}
		void apply_rotation() {
		}
};

class Brick {
	public:
		Vector2f pos;

		Point p1;
		Point p2;
		Point p3;
		Point p4;
		Point p5;
		
		Joint j1;
		Joint j2;
		Joint j3;
		Joint j4;

	public:
		Brick& operator=(const Brick &rhs) {
			if (this == &rhs) return *this;
			
			this->pos = rhs.pos;

			this->p1 = rhs.p1;
			this->p2 = rhs.p2;
			this->p3 = rhs.p3;
			this->p4 = rhs.p4;
			this->p5 = rhs.p5;

			this->p1.next = &p2;
			this->p2.next = &p3;
			this->p3.next = &p4;
			this->p4.next = &p5;
			this->p5.next = &p1;

			this->p5.prev = &p4;
			this->p4.prev = &p3;
			this->p3.prev = &p2;
			this->p2.prev = &p1;
			this->p1.prev = &p5;



			this->j1 = rhs.j1;
			
			this->j1.p1 = &p1;
			this->j1.p2 = &p2;
		
			return *this;
		}

		Brick() {}
		Brick(Vector2f pos, float length) {
			this->pos = pos;
			
			this->p1 = Point(Vector2f(length,0),1,0*(M_PI/180),true);
			this->p2 = Point(Vector2f(length,0),1,90*(M_PI/180),false);
			this->p3 = Point(Vector2f(length,0),1,90*(M_PI/180),false);
			this->p4 = Point(Vector2f(length,0),1,90*(M_PI/180),false);
			this->p5 = Point(Vector2f(length,0),1,90*(M_PI/180),false);

			this->p1.index = 1;
			this->p2.index = 2;
			this->p3.index = 3;
			this->p4.index = 4;
			this->p5.index = 5;

			p1.next = &p2;
			p2.next = &p3;
			p3.next = &p4;
			p4.next = &p5;
			p5.next = &p1;

			p5.prev = &p4;
			p4.prev = &p3;
			p3.prev = &p2;
			p2.prev = &p1;
			p1.prev = &p5;

			this->p1.place();
			this->p2.place();
			this->p3.place();
			this->p4.place();
			this->p5.place(true);


			this->j1 = Joint(&p1,&p2,length,90*(M_PI/180));
		}
	public:
		void draw() {
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
			glColor3f(1.0,1.0,1.0);
			
			glTranslatef(p1.pos.x,p1.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();

			glTranslatef(p2.pos.x,p2.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();


			glTranslatef(p3.pos.x,p3.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();

			glTranslatef(p4.pos.x,p4.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			
			glColor3f(0.0,1.0,0.0);
			glTranslatef(p5.pos.x,p5.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			
			
			
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);

			glTranslatef(pos.x,pos.y,0);

			glTranslatef(p1.pos.x,p1.pos.y,0);
			glBegin(GL_LINES);
				glVertex2f(0,0);
				glVertex2f(p2.pos.x,p2.pos.y);
			glEnd();
			glTranslatef(p2.pos.x,p2.pos.y,0);
			glBegin(GL_LINES);
				glVertex2f(0,0);
				glVertex2f(p3.pos.x,p3.pos.y);
			glEnd();
			glTranslatef(p3.pos.x,p3.pos.y,0);
			glBegin(GL_LINES);
				glVertex2f(0,0);
				glVertex2f(p4.pos.x,p4.pos.y);
			glEnd();
			glTranslatef(p4.pos.x,p4.pos.y,0);
			glBegin(GL_LINES);
				glVertex2f(0,0);
				glVertex2f(p1.pos.x,p1.pos.y);
			glEnd();
			

		}
		void project(Vector2f ext, float dt) {
			this->p1.project(ext,dt);
			this->p2.project(ext,dt);
			this->p3.project(ext,dt);
			this->p4.project(ext,dt);
			this->p5.project(ext,dt);
		}
		void solve(Vector2f ext, float dt) {
			p1.was_computed = false;
			p2.was_computed = false;
			p3.was_computed = false;
			p4.was_computed = false;
			p5.was_computed = false;
			
			this->j1.apply(ext,dt,1);




		}
		void correct(Vector2f ext, float dt) {
			this->p1.correct(ext,dt);
			this->p2.correct(ext,dt);
			this->p3.correct(ext,dt);
			this->p4.correct(ext,dt);

		}
};

class World {
	public:
		Array<Brick> bricks;
		std::list<Brick *> objects;

		Vector2f gravity;
		float dt;
		int32_t count;

		World() :
			gravity(0,0.098),
			dt(0.02),
			count(0)
		{}
	public:
		Brick* add_brick(Vector2f pos, float length) {
			size_t num;
			num = this->bricks.add_item(Brick(pos,length));
			this->objects.push_back(this->bricks.get_find_ref(num));
			this->bricks[count++] = Brick(pos,length);
			return this->bricks.get_find_ref(num);
		}
		void draw() {
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++
			) {
				(*it)->draw();
			}
			glLoadIdentity();
			glColor3f(0.0,1.0,0.0);
			glLineWidth(10);
			glBegin(GL_LINES);
				glVertex2f(R1.x,R1.y);
				glVertex2f(R2.x,R2.y);
			glEnd();
			glLoadIdentity();
		}
		void iterate() {
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++
			) {
				(*it)->project(gravity,dt);
			}

			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++
			) {
				(*it)->solve(gravity,dt);
			}
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++
			) {
				(*it)->correct(gravity,dt);
			}
		}
};


void test() {
	SDL_Event event;
	bool quit = false;
	
	Vector2f A(-50,-50);
	Vector2f B=A;


		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		glLoadIdentity();
		glColor3f(1.0,0.0,0.0);
		glLineWidth(3);
		glBegin(GL_LINES);
			glVertex2f(400,300);
			glVertex2f(400+A.x,300+A.y);
		glEnd();	
		float move = 5;
		for (int i=0; i < 360; i+=move) {
			B =rotate(B,-move* (M_PI/180));
			printf("%f\n",angleOfVector(A,B) * (180/M_PI));
			glLoadIdentity();
			glColor3f(1.0,1.0,1.0);
			glLineWidth(3);
			glBegin(GL_LINES);
				glVertex2f(400,300);
				glVertex2f(400+B.x,300+B.y);
			glEnd();	
			SDL_Delay(100);
		SDL_GL_SwapBuffers();
		}



	while (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;


	}
	exit(0);
}

int main (int argc, char **argv) {
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

	world.add_brick(Vector2f(300,200),100);

	//test();

	SDL_Event event;
	bool quit = false;


	while (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;

		world.iterate();

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		world.draw();

		SDL_GL_SwapBuffers();

	}
}
