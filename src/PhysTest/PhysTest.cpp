#include "PhysTest.h"

class Point {
	public:
		Vector2f pos;
		Vector2f proj_pos;
		Vector2f speed;
		float inv_mass;

		Point() : pos(0,0), proj_pos(0,0), speed(0,0), inv_mass(1) {}
		Point(Vector2f pos, float inv_mass) :
			pos(pos),
			proj_pos(pos),
			speed(0,0),
			inv_mass(inv_mass) {}

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
		float length;

		Joint() : p1(NULL), p2(NULL), stiffness(0), length(0) {}
		Joint(Point *p1, Point *p2, float length, float stiffness) :
			p1(p1),
			p2(p2),
			stiffness(stiffness),
			length(length) 
		{}
	
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
		void jointer(Point *rp1, Point *rp2) {
				float cat1 = (rp1->proj_pos.x - rp2->pos.x);
				float cat2 = (rp1->proj_pos.y - rp2->pos.y);
				
				cat1 = -cat1;
				cat2 = cat2;
				
				
				//if (cat1 < 50)
				//SDL_Delay(3);
	
				//printf("%f %f\n",cat1, cat2);
				
				float hyp = sqrt(cat1 * cat1 + cat2 * cat2);
				
				
				float sine = cat2 / hyp; 
				float cosine = cat1 / hyp;

				float res_x, res_y;
				
				res_x = (-length) * cosine - 0;
				res_y = -(-length) * sine + 0;

				//printf("%f %f\n",rp2->pos.x + res_x, rp2->pos.y + res_y);

				rp1->proj_pos = Vector2f(rp2->pos.x + res_x,rp2->pos.y + res_y);
		}
		void apply() {
			if (p1->pos != p1->proj_pos) {
				this->jointer(p1,p2);
			}
			if (p2->pos != p2->proj_pos) {
				this->jointer(p2,p1);
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
			p1(Vector2f(pos.x-length/2,pos.y),1),
			p2(Vector2f(pos.x+length/2,pos.y),1),
			p3(Vector2f(pos.x+length*1.5,pos.y),1),
			holder(&this->p1, &this->p2, length, 0.5) ,
			holder2(&this->p2, &this->p3, length, 0.5) 
		{
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
			this->holder.apply();
			this->holder2.apply();
		}
		void compute(Stick *obj) {
		}
		void compute(Plain *obj) {
		}
		void project_pt(Point *p, Vector2f ext, float dt) {
			p->speed = p->speed + ext * dt * p->inv_mass;
			p->proj_pos = p->pos + p->speed * dt;
		}
		void project(Vector2f ext,float dt) {
			//project_pt(&p1,ext,dt);
			project_pt(&p2,ext,dt);
			project_pt(&p3,ext,dt);
		}
		void update_pt(Point *p, float dt) {
			p->speed = (p->proj_pos - p->pos) / dt;
			p->pos = p->proj_pos;
		}
		void update(float dt) {
			//update_pt(&p1, dt);
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

		World() : gravity(0,10), dt(0.001) {}

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


	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,800, 600,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	World world;
	
	
	world.add_stick(Vector2f(500,100),100);
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
