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
		void draw() {
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();
			glLoadIdentity();
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
			Vector2f normal;
			Vector2f iV;
			Vector2f iPos;
			float mod =1;
			Point *a,*b,*c;
			if (direction > 0) {
				a = p1;
				b = p1->next;
				c = p1->next->next;
				for (int i=0; i < 3; i++) {
					bool amove = (a->pos != a->prev_pos);
					bool cmove = (c->pos != c->prev_pos);
					float r1 = (b->pos - a->pos).length();
					float r2 = (b->pos - c->pos).length();
					float diff1 = (r1-length);
					float diff2 = (r2-length);
					Vector2f normal_ab = (a->pos - b->pos).normalize();
					Vector2f normal_bc = (c->pos - b->pos).normalize();
					
					if (amove && cmove) {
						printf("pos\n");
					} else if (!amove && cmove) {
						if (diff2 < diff1) {
							b->F = normal_ab * diff1;
						} else {
							b->F = normal_ab * diff1;
						}
					} else if (amove && !cmove) {
						//printf("%f %f\n",diff1,diff2);
						if (diff1 < diff2) {
							b->F = normal_bc * diff2;
						} else {
							b->F = normal_bc * diff2;
						}
					} else if (!amove && !cmove) {
						b->pos = b->prev_pos;
					}
					R1 = b->pos;
					R2 = b->pos + normal;

					a = a->next;
					b = a->next;
					c = b->next;
				}
				a = p1->next;
				b = p1->next->next;
				c = p1->next->next->next;;
					float r = (a->pos - b->pos).length();
					float diff = (r-length);
					Vector2f normal = (a->pos - b->pos).normalize();
					a->F += normal * diff * -5; 
					b->F += normal * diff * 5;
			}
			
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
			
			this->p1 = Point(pos+Vector2f(0,0),1,180*(M_PI/180),true);
			this->p2 = Point(p1.pos+rotate(Vector2f(length,0),p1.angle),1,-0*(M_PI/180),false);
			this->p3 = Point(p2.pos+rotate(Vector2f(length,0),p2.angle),1,0*(M_PI/180),false);
			this->p4 = Point(p3.pos+rotate(Vector2f(length,0),p3.angle),1,0,true);

			p1.next = &p2;
			p2.next = &p3;
			p3.next = &p4;
			p4.next = &p1;

			p4.prev = &p3;
			p3.prev = &p2;
			p2.prev = &p1;
			p1.prev = &p4;


			this->j1 = Joint(&p1,&p2,length,90*(M_PI/180));
			this->j2 = Joint(&p2,&p3,length,90*(M_PI/180));
			this->j3 = Joint(&p3,&p4,length,90*(M_PI/180));
			this->j4 = Joint(&p4,&p1,length,90*(M_PI/180));
		}
	public:
		void draw() {
			this->p1.draw();
			this->p2.draw();
			this->p3.draw();
			this->p4.draw();

			this->j1.draw();
			this->j2.draw();
			this->j3.draw();
			//this->j4.draw();
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
