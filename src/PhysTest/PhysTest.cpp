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
};



class Brick {
	public:
		Vector2f pos;
		Vector2f rotation_center;
		std::vector<Point> points;
		float length;
		float radius;
		float angularity;
	public:
		Brick() {
		}
		Brick(Vector2f pos, float length, float angularity) {
			radius = length / sqrt(2);
			this->radius = radius; 
			this->length = length;
			this->pos = pos;
			float angles = 360*(M_PI/180) / angularity;
			Vector2f base(100,0);
			Vector2f horde = rotate(Vector2f(radius,0),-angularity);
			float diffangle = angleOfVector(horde - Vector2f(radius,0),base);

			for (int i=0; i < angles; i++) {
				if (i==1) {
					this->points.push_back(
						Point(radius,1,i*-angularity-diffangle,true)
					);
				} else {
					this->points.push_back(
						Point(radius,1,i*-angularity-diffangle,false)
					);
				}
				this->points.back().index = i;
			}
		}
	private:
		void solve_linear(std::vector<Point>::iterator it) {
			float diff = radius - it->pos.length();
			it->F = it->pos.normalize() * diff;
		}
		void solve_angular(
			std::vector<Point>::iterator it,
			std::vector<Point>::iterator ti
		) {
			float adiff = length-(it->pos - ti->pos).length();
			if (!ti->pinned && !it->pinned) {
				it->F = (it->pos - ti->pos).normalize() * adiff; 
				ti->F = (it->pos - ti->pos).normalize() * -adiff; 
			}
			///SDL_Delay(10);
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
				glLoadIdentity();
				glTranslatef(pos.x+it->pos.x,pos.y+it->pos.y,0);
				glBegin(GL_POLYGON);
					for (int i=0; i <= 360; i++) {
						glVertex2f(10*cos(i),10*sin(i));
					}
				glEnd();
			}	
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
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
		void solve(float dt) {
			for (
				std::vector<Point>::iterator it = points.begin(); 
				it != points.end(); 
				it++
			) {
				solve_linear(it);
			}
			this->correct(dt);
			std::vector<Point>::iterator it = points.begin();
			std::vector<Point>::iterator ti = it;
			ti++;
			for (
				; 
				it != points.end(); 
				it++,ti++
			) {
				if (ti == points.end())
					ti = points.begin();
				solve_angular(it,ti);
			}
		}
		void correct(float dt) {
			for (
				std::vector<Point>::iterator it = points.begin(); 
				it != points.end(); 
				it++
			) {
				it->correct(dt);
			}
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
		void add_brick(Vector2f pos, float length, float angularity) {
			size_t num;
			num = this->bricks.add_item(Brick(pos,length,angularity));
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
				(*it)->solve(dt);
			}
			for (
				std::list<Brick*>::iterator it = this->objects.begin();
				it != this->objects.end();
				it++) 
			{
				(*it)->correct(dt);
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

	world.add_brick(Vector2f(300,200),100,90*(M_PI/180));

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
