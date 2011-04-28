#include "PhysTest.h"

class Point {
	public:
		Vector2f pos;
		Vector2f V;
		Vector2f F;
		Vector2f U;
		Vector2f C;
		float mass;
		float inertia;
		bool pinned;
	public:
		Point() :
			pos(0,0),
			V(0,0),
			F(0,0),
			U(0,0),
			C(0,0),
			mass(0),
			inertia(0),
			pinned(false)
		{}
		Point(Vector2f pos, float mass, bool pinned) :
			pos(pos),
			V(0,0),
			F(0,0),
			U(0,0),
			C(0,0),
			mass(mass),
			pinned(pinned)
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
			this->pos += this->V * dt;

		}
		void correct(Vector2f ext, float dt) {
			this->V += F / mass * dt + U * dt;
			this->F = Vector2f(0,0);
			this->U = Vector2f(0,0);
		}
};

float tmp = 0;

class LinearJoin {
	public:
		Point *p1;
		Point *p2;
		float length;
	public:
		LinearJoin() {
			this->p1 = NULL;
			this->p2 = NULL;
			this->length = 0;
		}
		LinearJoin(Point *p1, Point *p2, float length) {
			this->p1 = p1;
			this->p2 = p2;
			this->length = length;
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
			Vector2f normal;
			float r1;
			normal = (p1->pos - p2->pos).normalize();
			r1 = (p2->pos - p1->pos).length();
			p2->F = normal * (r1-length);
		}
};

Vector2f R1,R2;


class AngularJoint {
	public:
		Point *p1;
		Point *p2;
		Point *p3;
		float angle;
	public:
		AngularJoint() {
			p1 = NULL;
			p2 = NULL;
			p3 = NULL;
			angle = 0;
		}
		AngularJoint(Point *p1, Point *p2, Point *p3, float angle) :
			p1(p1),
			p2(p2),
			p3(p3),
			angle(angle * (M_PI/180))
		{}
	public:
		void apply() {
			Vector2f normal;

			Vector2f d1 = p1->pos - p2->pos;
			Vector2f d2 = p3->pos - p2->pos;
			
			float full_angle = angleOfVector(d1,d2);
			printf("%f\n",full_angle * (180/M_PI));
			SDL_Delay(10);

				float r1 = full_angle - angle;
				normal = Vector2f(d2.y,-d2.x); 
				//p3->U = normal;
				R1 = p3->pos;
				R2 = p3->pos + normal;
			
			/*
	
			float full_angle = angleOfVector(d1,d2);

			Vector2f proj1 = rotate(Vector2f(d2.length(),0),angle);

			R1 = p3->pos;
			R2 = p3->pos + proj1;


			float a1 = angleOfVector(proj1,d1);

			printf("%f\n",a1 / (M_PI/180));
			SDL_Delay(1);

			//SDL_Delay(10);
			*/
			/*
			float r1,r2;
			Vector2f diff = p2->pos - p1->pos;
			Vector2f normal;
			r1 = angleOfVector(Vector2f(1,0),diff);
			//printf("%f %f \n",r1,angle);
			if (r1 != angle) {
				r2 = r1 - angle;
				Vector2f proj_pos = p1->pos + rotate(Vector2f(100,0),angle);
				//normal = Vector2f(diff.y,-diff.x).normalize();
				normal = (p1->pos - p2->pos) * r2;
				normal=Vector2f(-normal.y,normal.x);
				R1 = p1->pos;
				R2 = proj_pos;
				//p2->U = normal;
				p2->pos = proj_pos;
				printf(">> %f\n",r2);
	//			SDL_Delay(10);
			}
		*/
		}
};

class Brick {
	public:
		Vector2f pos;

		Point p1;
		Point p2;
		Point p3;
		Point p4;
		
		LinearJoin lj1;
		LinearJoin lj2;
		LinearJoin lj3;

		AngularJoint aj1;
		AngularJoint aj2;
		AngularJoint aj3;

	public:
		Brick& operator=(const Brick &rhs) {
			if (this == &rhs) return *this;

			this->p1 = rhs.p1;
			this->p2 = rhs.p2;
			this->p3 = rhs.p3;
			this->p4 = rhs.p4;

			this->lj1 = rhs.lj1;
			this->lj2 = rhs.lj2;
			this->lj3 = rhs.lj3;
			this->lj1.p1 = &p1;
			this->lj1.p2 = &p2;
			this->lj2.p1 = &p2;
			this->lj2.p2 = &p3;
			this->lj3.p1 = &p3;
			this->lj3.p2 = &p4;

			this->aj1 = rhs.aj1;
			//this->aj2 = rhs.aj2;
			//this->aj3 = rhs.aj3;
			this->aj1.p1 = &p1;
			this->aj1.p2 = &p2;
			this->aj1.p3 = &p3;
			//this->aj2.p1 = &p2;
			//this->aj2.p2 = &p3;
			//this->aj3.p1 = &p3;
			//this->aj3.p2 = &p4;


			return *this;
		}

		Brick() {}
		Brick(Vector2f pos, float length) {
			this->pos = pos;
			
			this->p1 = Point(pos+Vector2f(length*0,0),1,true);
			this->p2 = Point(pos+Vector2f(length*1,0),1,false);
			this->p3 = Point(pos+Vector2f(length*2,0),1,false);
			this->p4 = Point(pos+Vector2f(length*3,0),1,false);

			this->lj1 = LinearJoin(&p1,&p2,length);
			this->lj2 = LinearJoin(&p2,&p3,length);
			this->lj3 = LinearJoin(&p3,&p4,length);
			
			this->aj1 = AngularJoint(&p1,&p2,&p3,180);
			//this->aj2 = AngularJoint(&p2,&p3,0);
			//this->aj3 = AngularJoint(&p3,&p4,0);
		}
	public:
		void draw() {
			this->p1.draw();
			this->p2.draw();
			this->p3.draw();
			//this->p4.draw();

			this->lj1.draw();
			this->lj2.draw();
			//this->lj3.draw();
		}
		void project(Vector2f ext, float dt) {
			this->p1.project(ext,dt);
			this->p2.project(ext,dt);
			this->p3.project(ext,dt);
			//this->p4.project(ext,dt);
		}
		void solve(Vector2f ext, float dt) {
			this->lj1.apply(ext,dt);
			this->lj2.apply(ext,dt);
			//this->lj3.apply(ext,dt);

			this->aj1.apply();
			//this->aj2.apply();
			//this->aj3.apply();
		}
		void correct(Vector2f ext, float dt) {
			this->p1.correct(ext,dt);
			this->p2.correct(ext,dt);
			this->p3.correct(ext,dt);
			//this->p4.correct(ext,dt);
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

	world.add_brick(Vector2f(300,100),100);

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
