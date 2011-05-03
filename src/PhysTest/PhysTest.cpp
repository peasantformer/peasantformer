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
			if (impulse > 0.0001) {
				V += normal * impulse * (1/mass);
			}
		}
};



class Brick {
	public:
		Vector2f pos;
		Vector2f V;
		Vector2f rotation_center;
		std::vector<Point> points;
		float inertia;
		float mass;
		float length;
		float radius;
		float angularity;
		float width,height;
	public:
		Brick() {
		}
		Brick(Vector2f pos, float width, float height, bool pinned, float angle) {
			this->mass = 1;
			this->inertia = mass * width * height;
			this->width = width;
			this->height = height;
			this->points.push_back(
				Point(pos + rotate(Vector2f(-width/2,-height/2),angle),1,false)
			);
			points.back().index = 1;
			this->points.push_back(
				Point(pos + rotate(Vector2f(0, -height/2),angle),1,pinned)
			);
			this->points.push_back(
				Point(pos + rotate(Vector2f(width/2, -height/2),angle),1,pinned)
			);
			this->points.push_back(
				Point(pos + rotate(Vector2f(width/2, 0),angle),1,pinned)
			);
			points.back().index = 2;
			this->points.push_back(
				Point(pos + rotate(Vector2f(width/2, height/2),angle),1,pinned)
			);
			this->points.push_back(
				Point(pos + rotate(Vector2f(0, height/2),angle),1,pinned)
			);
			points.back().index = 3;
			this->points.push_back(
				Point(pos + rotate(Vector2f(-width/2, height/2),angle),1,pinned)
			);
			this->points.push_back(
				Point(pos + rotate(Vector2f(-width/2, 0),angle),1,pinned)
			);
			points.back().index = 4;
			
		}
	private:
		void solve_linear(
			Point *it,
			Point *ti,
			float len
		) {
			Vector2f dist = it->pos - ti->pos;
			float diff = len - dist.length();
				it->F += dist.normalize() * diff * 1000;
				ti->F += dist.normalize() * -1 * diff * 1000;
		}
		float calculate_impulse(Vector2f R, Vector2f normal, Brick *obj) {
			Vector2f R1 = R - pos;
			Vector2f R2 = R - obj->pos;
			float Vab = (V + obj->V).length();
	
			printf("%f\n",(R - pos).length());
			SDL_Delay(50);
		/*
			Vector2f R1 = R - pos;
			Vector2f R2 = R - obj->pos;
			float Vab = (V + obj->V).length();
			float Z1 = (normal.y * R1.x - normal.x * R1.y) * (1/inertia);
			float Z2 = (normal.y * R2.x - normal.x * R2.y) * (1/obj->inertia);

			float J = normal.x *
				(
					normal.x * (1/mass) - R1.y * Z1 + normal.x * (1/obj->mass) + R2.y * Z2
				)
				+ normal.y * (normal.y * (1/mass) + R1.x * Z1 * (1/inertia)
				+ normal.y * (1/obj->mass) - R2.x * Z2 * (1/obj->inertia)
				)
			;
			return (1 - (1+1) * Vab) / J;
			*/
			return 0;
		}
	public:
		void draw() {
			glColor3f(0,0,1);
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10*cos(i),10*sin(i));
				}
			glEnd();

			glColor3f(1.0,1.0,1.0);
			for (
				std::vector<Point>::iterator it = points.begin(); 
				it != points.end(); 
				it++
			) {
				switch (it->index) {
					case 1:
						glColor3f(1,0,0);
						break;
					case 2:
						glColor3f(0,1,0);
						break;
					case 3:
						glColor3f(0,0,1);
						break;
					case 4:
						glColor3f(1,1,1);
						break;
				}
				glLoadIdentity();
				glTranslatef(it->pos.x,it->pos.y,0);
				glBegin(GL_POLYGON);
					for (int i=0; i <= 360; i++) {
						glVertex2f(10*cos(i),10*sin(i));
					}
				glEnd();
			}	
			glLoadIdentity();
			glBegin(GL_LINE_LOOP);
			for (
				std::vector<Point>::iterator it = points.begin(); 
				it != points.end();
				it++
			) {
					glVertex2f(it->pos.x,it->pos.y);
			}
			glEnd();
		}
		void project(Vector2f ext, float dt) {
			for (
				std::vector<Point>::iterator it = points.begin(); 
				it != points.end(); 
				it++
			) {
				it->project(ext,dt);
			}
		}
		void self_solve(float dt) {
			Point *p1 = &this->points[0];
			Point *p2 = &this->points[1];
			Point *p3 = &this->points[2];
			Point *p4 = &this->points[3];
			Point *p5 = &this->points[4];
			Point *p6 = &this->points[5];
			Point *p7 = &this->points[6];
			Point *p8 = &this->points[7];


			solve_linear(p1,p2,width/2);
			solve_linear(p2,p3,width/2);
			solve_linear(p3,p4,height/2);
			solve_linear(p4,p5,height/2);
			solve_linear(p5,p6,width/2);
			solve_linear(p6,p7,width/2);
			solve_linear(p7,p8,height/2);
			solve_linear(p8,p1,height/2);
			
			solve_linear(p2,p6,height);
			solve_linear(p8,p4,width);
			solve_linear(p1,p5,sqrt(width*width + height*height));
			solve_linear(p3,p7,sqrt(width*width + height*height));
			solve_linear(p2,p5,sqrt(width/2 * width/2 + height * height));
			solve_linear(p3,p6,sqrt(width/2 * width/2 + height * height));
			solve_linear(p1,p6,sqrt(width/2 * width/2 + height * height));
			solve_linear(p2,p7,sqrt(width/2 * width/2 + height * height));
	

			solve_linear(p4,p2,sqrt(width/2 * width/2 + height/2 * height/2));
			solve_linear(p4,p6,sqrt(width/2 * width/2 + height/2 * height/2));
			
			solve_linear(p8,p2,sqrt(width/2 * width/2 + height/2 * height/2));
			solve_linear(p8,p6,sqrt(width/2 * width/2 + height/2 * height/2));
			
			
		}
		void solve(
			Brick* ti
			) {
			Point *p1 = &this->points[0];
			Point *p2 = &this->points[1];
			Point *p3 = &this->points[2];
			Point *p4 = &this->points[3];
			Point *p5 = &this->points[4];
			Point *p6 = &this->points[5];
			Point *p7 = &this->points[6];
			Point *p8 = &this->points[7];

			Point *n1 = &ti->points[0]; // left - upper
			Point *n2 = &ti->points[1]; // middle - upper
			Point *n3 = &ti->points[2]; // right - upper
			Point *n4 = &ti->points[3]; // right - middle
			Point *n5 = &ti->points[4]; // right - bottom
			Point *n6 = &ti->points[5]; // midle - bottom
			Point *n7 = &ti->points[6]; // left - bottom
			Point *n8 = &ti->points[7]; // left - midle

			Vector2f R;
			float impulse;
	
			if (lines_intersect(p2->pos,p6->pos,n1->pos,n3->pos,&R)) {
					impulse = calculate_impulse(R,Vector2f(0,1),ti) * -1;
					p6->apply_impulse(impulse,Vector2f(0,-1));
			}
			if (lines_intersect(p8->pos,p7->pos,n1->pos,n3->pos,&R)) {
					impulse = calculate_impulse(R,Vector2f(0,1),ti) * -1;
					p7->apply_impulse(impulse,Vector2f(0,-1));
			}
			if (lines_intersect(p4->pos,p5->pos,n1->pos,n3->pos,&R)) {
					impulse = calculate_impulse(R,Vector2f(0,1),ti) * -1;
					p5->apply_impulse(impulse,Vector2f(0,-1));
			}


			/*
			Vector2f R;

			if (lines_intersect(p1->pos,p4->pos,n1->pos,n2->pos,&R)) {
				float impulse =  calculate_impulse(R,Vector2f(0,1),ti) * -1;
				p4->apply_impulse(impulse,Vector2f(0,-1));
			}
			if (lines_intersect(p2->pos,p3->pos,n1->pos,n2->pos,&R)) {
				float impulse =  calculate_impulse(R,Vector2f(0,1),ti) * -1;
				p3->apply_impulse(impulse,Vector2f(0,-1));
			}
			if (lines_intersect(p2->pos,p3->pos,n1->pos,n2->pos,&R)) {
			//	p3->F += (R-p3->pos).normalize() * 100;
			}
			*/
		}
		void correct(float dt) {
			for (
				std::vector<Point>::iterator it = points.begin(); 
				it != points.end(); 
				it++
			) {
				it->correct(dt);
			}
			Point *p1 = &this->points[0];
			Point *p3 = &this->points[2];
			Point *p5 = &this->points[4];
			Point *p7 = &this->points[6];
			Vector2f prev_pos = pos;
			lines_intersect(p1->pos,p5->pos,p3->pos,p7->pos,&pos);
			this->V =(pos-prev_pos) / dt;
		}
};



class World {
	public:
		Array<Brick> bricks;
		std::list<Brick *> objects;
		Vector2f gravity;
		float dt;
	public:
		World() :
			gravity(0,0.098),
			dt(0.02)
		{}
	public:
		void add_brick(Vector2f pos, float width, float height, bool pinned = false, float angle = 0) {
			size_t num;
			num = this->bricks.add_item(Brick(pos,width,height,pinned,angle));
			this->objects.push_back(this->bricks.get_find_ref(num));

		}
		void iterate() {
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->project(gravity,dt);
			}
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->self_solve(dt);
				(*it)->correct(dt);
			}
			
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				std::list<Brick*>::iterator ti = it;
				ti++;
				for (; ti != this->objects.end(); ti++) {
					(*it)->solve(*ti);
				}
			}
			
		}
		void draw() {
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->draw();
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


	world.add_brick(Vector2f(300,200),200,100,false,-0*(M_PI/180));
	
	
	world.add_brick(Vector2f(400,600),800,200,true);
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
