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
			was_computed(false)
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
			was_computed(false)
		{
			inertia = mass * 50 * 50;
		}
	public:
		void place() {
			pos = rotate(pos,angle);
			prev_pos = pos;

		}
		void draw() {
		}
		void project(Vector2f ext, float dt) {
			if (this->pinned) return;
			this->V += ext * dt + C * dt;
			this->prev_pos = this->pos;
			this->pos += this->V * dt;

		}
		void correct(Vector2f ext, float dt) {
			if (this->pinned) return;
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
		float radius;
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
		void apply(Vector2f ext, float dt) {
			Point *a,*b;
<<<<<<< HEAD
			Vector2f normal;
			a = p1;
			b = p1->next;
			
			for (int i=0; i < 4; i++) {
				float adiff = length - a->pos.length();
				float bdiff = length - b->pos.length();
				bool amoved = a->pos != a->prev_pos;
				bool bmoved = b->pos != b->prev_pos;
				//bool amoved = !a->pinned;
				//bool bmoved = !b->pinned;
				
				if (amoved && bmoved) {
					normal = (a->pos - b->pos).normalize();
					//a->F = normal * adiff;
					//b->F = normal * bdiff * -1;
=======
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
>>>>>>> HEAD@{1}
					a->F = a->pos.normalize() * adiff;
					b->F = b->pos.normalize() * bdiff;
					//a->F = a->pos.normalize() * adiff;
					//b->F = b->pos.normalize() * bdiff;
				} else if (amoved && !bmoved) {
<<<<<<< HEAD
					//normal = (a->pos - b->pos).normalize();
					//a->F = normal * adiff;
					a->F = a->pos.normalize() * adiff;
=======
					

					Vector2f realpos = 
						a->prev->prev->prev->pos +
						a->prev->prev->pos +
						a->prev->pos;

					Vector2f diff = (realpos + a->pos);
					float fdiff = diff.length();
					//a->F = diff * -length;
					a->F = a->pos.normalize() * adiff;

>>>>>>> HEAD@{1}
				} else if (!amoved && bmoved) {
					//normal = (b->pos - a->pos).normalize();
					//b->F = normal * bdiff;
					b->F = b->pos.normalize() * bdiff;
				}

				a = a->next;
				b = a->next;
			}
			a = p1;
			b = p1->prev;
			float sum = 0;
			sum += a->pos.length();
			sum += a->next->pos.length();
			sum += a->next->next->pos.length();
			sum += a->next->next->next->pos.length();
			float diff = sum - length * 4;
			if (diff < 0) {
				//b/ += 
			}
			printf("%f\n",sum);
		}
		void apply_rotation(Vector2f ext, float dt) {
		}
};

class Brick {
	public:
		Vector2f pos;

		Point p1;
		Point p2;
		Point p3;
		Point p4;
		
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

			this->p1.next = &p2;
			this->p2.next = &p3;
			this->p3.next = &p4;
			this->p4.next = &p1;

			this->p4.prev = &p3;
			this->p3.prev = &p2;
			this->p2.prev = &p1;
			this->p1.prev = &p4;



			this->j1 = rhs.j1;
			this->j2 = rhs.j2;
			this->j3 = rhs.j3;
			this->j4 = rhs.j4;
			
			this->j1.p1 = &p1;
			this->j1.p2 = &p2;
			this->j2.p1 = &p2;
			this->j2.p2 = &p3;
			this->j3.p1 = &p3;
			this->j3.p2 = &p4;
			this->j4.p1 = &p4;
			this->j4.p2 = &p1;
		
			return *this;
		}

		Brick() {}
		Brick(Vector2f pos, float length) {
			this->pos = pos;
			
<<<<<<< HEAD
			float radius = length*sqrt(2)/2;

			this->p1 = Point(Vector2f(radius,0),1,-45*(M_PI/180),true);
			this->p2 = Point(Vector2f(radius,0),1,-135*(M_PI/180),false);
			this->p3 = Point(Vector2f(radius,0),1,135*(M_PI/180),false);
			this->p4 = Point(Vector2f(radius,0),1,45*(M_PI/180),false);
=======
			this->p1 = Point(Vector2f(length,0),1,0*(M_PI/180),true);
			this->p2 = Point(Vector2f(length,0),1,90*(M_PI/180),false);
			this->p3 = Point(Vector2f(length,0),1,90*(M_PI/180),false);
			this->p4 = Point(Vector2f(length,0),1,90*(M_PI/180),false);
>>>>>>> HEAD@{1}


			this->p1.index = 1;
			this->p2.index = 2;
			this->p3.index = 3;
			this->p4.index = 4;

			p1.next = &p2;
			p2.next = &p3;
			p3.next = &p4;
			p4.next = &p1;

			p4.prev = &p3;
			p3.prev = &p2;
			p2.prev = &p1;
			p1.prev = &p4;

			this->p1.place();
			this->p2.place();
			this->p3.place();
			this->p4.place();


			this->j1 = Joint(&p1,&p2,radius,90*(M_PI/180));
			this->j2 = Joint(&p2,&p3,radius,90*(M_PI/180));
			this->j3 = Joint(&p3,&p4,radius,90*(M_PI/180));
			this->j4 = Joint(&p4,&p1,radius,90*(M_PI/180));
		}
	public:
		void draw() {
			glColor3f(0.0,0.0,1.0);
			glLoadIdentity();
			
			glTranslatef(pos.x,pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			glLoadIdentity();

			glColor3f(1.0,1.0,1.0);
			glLoadIdentity();
			
			glTranslatef(pos.x+p1.pos.x,pos.y+p1.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			glLoadIdentity();

			glTranslatef(pos.x+p2.pos.x,pos.y+p2.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			glLoadIdentity();


			glTranslatef(pos.x+p3.pos.x,pos.y+p3.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			glLoadIdentity();

			glTranslatef(pos.x+p4.pos.x,pos.y+p4.pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			
			
			
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);

			glTranslatef(pos.x,pos.y,0);
<<<<<<< HEAD
			
			glBegin(GL_LINE_LOOP);
				glVertex2f(
					p1.pos.x,
					p1.pos.y
				);
				glVertex2f(
					p2.pos.x,
					p2.pos.y
				);
				glVertex2f(
					p3.pos.x,
					p3.pos.y
				);
				glVertex2f(
					p4.pos.x,
					p4.pos.y
				);
=======

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
>>>>>>> HEAD@{1}
			glEnd();
			

		}
		void project(Vector2f ext, float dt) {
			this->p1.project(ext,dt);
			this->p2.project(ext,dt);
			this->p3.project(ext,dt);
			this->p4.project(ext,dt);
		}
		void solve(Vector2f ext, float dt) {
			p1.was_computed = false;
			p2.was_computed = false;
			p3.was_computed = false;
			p4.was_computed = false;

			
			this->j1.apply(ext,dt);
			this->j1.apply_rotation(ext,dt);



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
