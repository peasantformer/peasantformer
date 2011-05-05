#include "PhysTest.h"

Vector2f R1,R2;

class Point {
	public:
		Vector2f pos;
		float length;
		Vector2f V;
		Vector2f F;
		Vector2f U;
		float mass;
		float angle;
		float impulse;
		bool pinned;
		int index;
	public:
		Point() :
			pos(0,0),
			length(0),
			V(0,0),
			F(0,0),
			U(0,0),
			mass(0),
			angle(0),
			pinned(false),
			index(0)
		{}
		Point(Vector2f pos, float mass, bool pinned) :
			pos(pos),
			length(length),
			V(0,0),
			F(0,0),
			U(0,0),
			mass(mass),
			angle(0),
			pinned(pinned),
			index(0)
		{}
		Point(float length, float mass, float angle, bool pinned) :
			pos(0,0),
			length(length),
			V(0,0),
			F(0,0),
			U(0,0),
			mass(mass),
			angle(0),
			pinned(pinned),
			index(0)
		{
			this->pos = rotate(Vector2f(length,0),angle);
			this->impulse = 0;
		}
	public:
		void project(Vector2f ext, float dt) {
			if (pinned) return;
			this->V += ext * dt;
			this->pos += V * dt;
		}
		void correct(float dt) {
			if (pinned) return;
			this->V += F / mass * dt;
			this->F = Vector2f(0,0);
		}
		void apply_impulse(float impulse, Vector2f normal) {
			V += normal * impulse;
		}
};


class Triangle {
	public:
		Vector2f pos;
		Point A,B,C;
		float a,b,c;
		float ABC, BCA, CAB;
		float R;
		float r;
		bool pinned;
		float angle;
	public:
		Triangle() {
		}
		Triangle(Vector2f pos, float a, float b, float c, bool pinned, float angle) {
			this->pos = pos;
			this->a = a;
			this->b = b;
			this->c = c;
			this->R = a*b*c / sqrt((a+b+c) * (-a+b+c) * (a-b+c) * (a+b-c));
			float p = (a+b+c)/2;
			this->r = sqrt(((p-a) * (p-b) * (p-c)) / p);
			this->ABC = acos((a*a + b*b - c*c) / (2*a*b));
			this->BCA = acos((a*a + c*c - b*b) / (2*a*c));
			this->CAB = acos((b*b + c*c - a*a) / (2*b*c));
			

			this->A = Point(pos, 1, pinned);
			this->B = Point(A.pos + rotate(Vector2f(c,0),angle-CAB), 1, pinned);
			this->C = Point(B.pos + rotate(Vector2f(a,0),angle+ABC), 1, pinned);
			Vector2f d = rotate(Vector2f((a+b-c)/2,0),angle);
			Vector2f dv = rotate(Vector2f(r,0),angle);
			dv = Vector2f(-dv.y,dv.x);
			
			A.pos -= (d - dv);
			B.pos -= (d - dv);
			C.pos -= (d - dv);
			

		
		}
	public:
		void project(Vector2f ext, float dt) {
			
			this->A.project(ext,dt);
			this->B.project(ext,dt);
			this->C.project(ext,dt);


		}
		void self_solve(float dt) {
			Vector2f BC = (B.pos - C.pos);
			Vector2f AC = (A.pos - C.pos);
			Vector2f AB = (A.pos - B.pos);
			
			float real_a = BC.length();
			float real_b = AC.length();
			float real_c = AB.length();
			
			float a_diff = real_a - a;
			float b_diff = real_b - b;
			float c_diff = real_c - c;


			if (fabs(b_diff) > 0.001) {
				this->A.F -= AC.normalize() * b_diff /2 /dt *A.mass;
				this->C.F += AC.normalize() * b_diff /2 /dt *C.mass;
			}
			if (fabs(c_diff) > 0.001) {
				this->A.F -= AB.normalize() * c_diff /2 /dt *A.mass;
				this->B.F += AB.normalize() * c_diff /2 /dt *B.mass;
			}
			if (fabs(a_diff) > 0.001) {
				this->B.F -= BC.normalize() * a_diff /2 /dt *B.mass;
				this->C.F += BC.normalize() * a_diff /2 /dt *C.mass;
			}
		}
		void correct(float dt) {
			this->A.correct(dt);
			this->B.correct(dt);
			this->C.correct(dt);
		}
		void solve(Triangle *ti) {
			Vector2f Res;
			if (lines_intersect(A.pos,B.pos,ti->A.pos,ti->C.pos,&Res)) {
				A.pos =Res;
			}
			if (lines_intersect(B.pos,C.pos,ti->A.pos,ti->C.pos,&Res)) {
				C.pos =Res;
			}
		}
		void draw() {
			glColor3f(1.0,0.0,0.0);
			glLoadIdentity();
			glTranslatef(A.pos.x,A.pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				glVertex2f(10*cos(i),10*sin(i));
			}
			glEnd();
			glColor3f(0.0,1.0,0.0);
			glLoadIdentity();
			glTranslatef(B.pos.x,B.pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				glVertex2f(10*cos(i),10*sin(i));
			}
			glEnd();
			glColor3f(0.0,0.0,1.0);
			glLoadIdentity();
			glTranslatef(C.pos.x,C.pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				glVertex2f(10*cos(i),10*sin(i));
			}
			glEnd();
			glLoadIdentity();
			glColor3f(1,1,1);
			glBegin(GL_LINE_LOOP);
				glVertex2f(A.pos.x,A.pos.y);
				glVertex2f(B.pos.x,B.pos.y);
				glVertex2f(C.pos.x,C.pos.y);
			glEnd();
			/*
			glColor3f(0.0,0.0,1.0);
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				glVertex2f(10*cos(i),10*sin(i));
			}
			glEnd();
			*/
		}
};


class World {
	public:
		Array<Triangle> triamgles;
		std::list<Triangle *> objects;
		Vector2f gravity;
		float dt;
	public:
		World() :
			gravity(0,0.098),
			dt(0.02)
		{}
	public:
		void add_triangle(Vector2f pos, float a, float b, float c, bool pinned = false, float angle = 0) {
			size_t num;
			num = this->triamgles.add_item(Triangle(pos,a,b,c,pinned,angle));
			this->objects.push_back(this->triamgles.get_find_ref(num));

		}
		void iterate() {
			for (
				std::list<Triangle*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->project(gravity,dt);
			}
			for (
				std::list<Triangle*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->self_solve(dt);
				(*it)->correct(dt);
			}
			
			for (
				std::list<Triangle*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				std::list<Triangle*>::iterator ti = it;
				ti++;
				for (; ti != this->objects.end(); ti++) {
					(*it)->solve(*ti);
				}
			}
			
		}
		void draw() {
			for (
				std::list<Triangle*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->draw();
			}
			glLoadIdentity();
			glColor3f(1,0,0);
			glBegin(GL_LINES);
				glVertex2f(R1.x,R1.y);
				glVertex2f(R2.x,R2.y);
			glEnd();
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

	world.add_triangle(Vector2f(300,200),100,100,100,false,0*(M_PI/180));

	world.add_triangle(Vector2f(400,700),500,800,500,true,-180*(M_PI/180));
	
	//world.add_triangle(Vector2f(400,600),800,200,true);
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
