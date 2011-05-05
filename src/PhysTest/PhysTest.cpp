#include "PhysTest.h"

Vector2f R1,R2;

class Point {
	public:
		Vector2f pos;
		Vector2f next_pos;
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
			next_pos(0,0),
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
			next_pos(pos),
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
			this->next_pos = pos;
			this->impulse = 0;
		}
	public:
		void project(Vector2f ext, float dt) {
			if (pinned) return;
			this->V += ext * dt;
			this->pos += V * dt;
			this->next_pos = pos;
		}
		void correct(float dt) {
			if (pinned) return;
			this->next_pos = pos;
			this->V += F / mass * dt;
			this->F = Vector2f(0,0);
			this->next_pos += V * dt;
		}
		void apply_impulse(float impulse, Vector2f normal) {
			V += normal * impulse;
		}
};


class Triangle {
	public:
		Vector2f pos;
		Point A,B,C;
		Vector2f V;
		float a,b,c;
		float ABC, BCA, CAB;
		float R;
		float r;
		bool pinned;
		float angle;
		float mass;
		float inertia;
	public:
		Triangle() {
		}
		Triangle(Vector2f pos, float a, float b, float c, bool pinned, float angle) {
			this->V = Vector2f(0,0);
			this->pos = pos;
			this->a = a;
			this->b = b;
			this->c = c;
			this->R = a*b*c / sqrt((a+b+c) * (-a+b+c) * (a-b+c) * (a+b-c));
			float p = (a+b+c)/2;
			this->mass = p;
			this->inertia = mass;
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
			A.next_pos = A.pos;
			B.next_pos = B.pos;
			C.next_pos = C.pos;
		}
	public:
		void project(Vector2f ext, float dt) {
			
			this->A.project(ext,dt);
			this->B.project(ext,dt);
			this->C.project(ext,dt);


		}
		void self_solve(float dt) {
			float real_p;
			float realdiff;
			float p = a + b + c;
			int i=0;
			do {
			Vector2f BC = (B.next_pos - C.next_pos);
			Vector2f AC = (A.next_pos - C.next_pos);
			Vector2f AB = (A.next_pos - B.next_pos);
			
			float real_a = BC.length();
			float real_b = AC.length();
			float real_c = AB.length();
			
			float a_diff = real_a - a;
			float b_diff = real_b - b;
			float c_diff = real_c - c;
			


			if (fabs(b_diff) > 0.000) {
				this->A.F -= AC.normalize() * b_diff /2 /dt *A.mass;
				this->C.F += AC.normalize() * b_diff /2 /dt *C.mass;
			}
			if (fabs(c_diff) > 0.000) {
				this->A.F -= AB.normalize() * c_diff /2 /dt *A.mass;
				this->B.F += AB.normalize() * c_diff /2 /dt *B.mass;
			}
			if (fabs(a_diff) > 0.000) {
				this->B.F -= BC.normalize() * a_diff /2 /dt *B.mass;
				this->C.F += BC.normalize() * a_diff /2 /dt *C.mass;
			}
			
			this->A.correct(dt);
			this->B.correct(dt);
			this->C.correct(dt);
			
			real_a = (B.next_pos - C.next_pos).length();
			real_b = (A.next_pos - C.next_pos).length();
			real_c = (A.next_pos - B.next_pos).length();

			printf("%f\n",real_a);

			real_p = real_a + real_b + real_c;
			realdiff = 0;
			realdiff += fabs(real_a - a);
			realdiff += fabs(real_b - b);
			realdiff += fabs(real_c - c);
	//		printf("%f\n",real_p);
			i++;
			} while (fabs(real_p - p) > 1 || realdiff > 1);
			printf("%d\n",i);

		}
		void correct(float dt) {
			Vector2f prevpos = this->pos;
			Vector2f n1 = A.next_pos;
			Vector2f n2 = C.next_pos + rotate(Vector2f(-a/2,0),BCA);
			Vector2f p1 = B.next_pos;
			Vector2f p2= A.next_pos + rotate(Vector2f(b/2,0),angle);
			lines_intersect(n1,n2,p1,p2,&this->pos);
			this->V = (prevpos-pos) / dt;


		}
		void solve(Triangle *ti) {
			Vector2f Res;
			Vector2f diff;
			Vector2f imp =  (V * mass / inertia) - (ti->V * ti->mass / ti->inertia) / 2;
			if (lines_intersect(A.next_pos,B.next_pos,ti->A.next_pos,ti->C.next_pos,&Res)) {
				diff = (Res - A.next_pos);
				A.pos = Res;
				A.next_pos = Res;
				A.V -= (A.V - ti->V) + Vector2f(0,-1) * imp;
			}
			if (lines_intersect(B.next_pos,C.next_pos,ti->A.next_pos,ti->C.next_pos,&Res)) {
				diff = (Res - C.next_pos);
				C.pos = Res;
				C.next_pos = Res;
				C.V -= (C.V - ti->V) + Vector2f(0,-1) * imp;
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
			glColor3f(0.0,0.0,1.0);
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				glVertex2f(10*cos(i),10*sin(i));
			}
			glEnd();
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
