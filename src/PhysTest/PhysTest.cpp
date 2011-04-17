#include "PhysTest.h"

class Point {
	public:
		Vector2f pos;
		Vector2f proj_pos;
		Vector2f speed;
		float inv_mass;
		bool was_computed;

		Point() : pos(0,0), proj_pos(0,0), speed(0,0), inv_mass(1), was_computed(false) {}
		Point(Vector2f pos, float inv_mass) :
			pos(pos),
			proj_pos(pos),
			speed(0,0),
			inv_mass(inv_mass),
			was_computed(false) {}

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
		Point *p3;
		float stiffness;
		float length1;
		float length2;
		bool upper;

		Joint() : 
			p1(NULL), 
			p2(NULL), 
			p3(NULL), 
			stiffness(0), 
			length1(0), 
			length2(0),
			upper(true)
		{}
		Joint(Point *p1, Point *p2, Point *p3, float length1, float length2, float stiffness, bool upper) :
			p1(p1),
			p2(p2),
			p3(p3),
			stiffness(stiffness),
			length1(length1),
			length2(length2),
			upper(upper)
		{

		}
	
		void draw() {
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);
			glLineWidth(50);
			glBegin(GL_LINES);
				glVertex2f(p1->pos.x,p1->pos.y);
				glVertex2f(p2->pos.x,p2->pos.y);
				glVertex2f(p2->pos.x,p2->pos.y);
				glVertex2f(p3->pos.x,p3->pos.y);
			glEnd();
			glLoadIdentity();
		}
		Vector2f jointer(Point *rp1, Point *rp2) {
				/*
				float cat1 = (rp1->proj_pos.x - rp2->pos.x);
				float cat2 = (rp1->proj_pos.y - rp2->pos.y);
				
				cat1 = -cat1;
				cat2 = cat2;
				
				float hyp = sqrt(cat1 * cat1 + cat2 * cat2);
				
				
				float sine = cat2 / hyp; 
				float cosine = cat1 / hyp;
				
				printf("%f %f\n",sine,cosine);

				float res_x, res_y;
				
				res_x = (-length) * cosine - 0;
				res_y = -(-length) * sine + 0;
				
				return Vector2f(rp2->pos.x + res_x,rp2->pos.y + res_y);
				*/
				return Vector2f(0,0);
		}
		void apply() {
			float catf1 = (p1->proj_pos.x - p2->pos.x);
			float catf2 = (p1->proj_pos.y - p2->pos.y);
			float cats1 = (p2->proj_pos.x - p1->pos.x);
			float cats2 = (p2->proj_pos.y - p1->pos.y);
			
			float catss1 = (p2->proj_pos.x - p3->pos.x);
			float catss2 = (p2->proj_pos.y - p3->pos.y);
			
			float catt1 = (p3->proj_pos.x - p2->pos.x);
			float catt2 = (p3->proj_pos.y - p2->pos.y);
			
			catf1 = -catf1;
			cats1 = -cats1;
			catss1 = -catss1;
			catt1 = -catt1;
			
			float hypf = sqrt(catf1 * catf1 + catf2 * catf2);
			float hyps = sqrt(cats1 * cats1 + cats2 * cats2);
			
			float hypss = sqrt(catss1 * catss1 + catss2 * catss2);
			
			float hypt = sqrt(catt1 * catt1 + catt2 * catt2);
			
			float sinef = catf2 / hypf; 
			float cosinef = catf1 / hypf;
			float sines = cats2 / hyps; 
			float cosines = cats1 / hyps;
			
			float siness = catss2 / hypss; 
			float cosiness = catss1 / hypss;
			
			float sinet = catt2 / hypt; 
			float cosinet = catt1 / hypt;
			
			float res_xf = (-length1) * cosinef - 0;
			float res_yf = (length1) * sinef + 0;
			float res_xs = (-length1) * cosines - 0;
			float res_ys = (length1) * sines + 0;
			
			float res_xss = (-length2) * cosiness - 0;
			float res_yss = (length2) * siness + 0;
			
			float res_xt = (-length2) * cosinet - 0;
			float res_yt = (length2) * sinet + 0;
			
			if (p1->proj_pos != p1->pos && p1->was_computed == false) {
				p1->proj_pos = Vector2f(p2->pos.x + res_xf, p2->pos.y + res_yf);
				p1->was_computed = true;
			}
			
			if (p2->proj_pos != p2->pos && p2->was_computed == false) {
				//printf("%f\n",);
				if (!upper)
					p2->proj_pos = Vector2f(p3->pos.x + res_xss, p3->pos.y + res_yss);
				else 
					p2->proj_pos = Vector2f(p1->pos.x + res_xs, p1->pos.y + res_ys);
				p2->was_computed = true;
			}
			
			if (p3->proj_pos != p3->pos && p3->was_computed == false) {
				p3->proj_pos = Vector2f(p2->pos.x + res_xt, p2->pos.y + res_yt);
				p3->was_computed = true;
			}

			//if (p2->proj_pos != p2->pos)
			//p2->proj_pos = Vector2f(p2->pos.x + res_xs, p2->pos.y + res_ys);
			
			//p2->proj_pos = Vector2f(p1->pos.x + res_xf,p1->pos.y + res_yf);
			//p3->proj_pos = Vector2f(p2->pos.x + res_xs,p2->pos.y + res_ys);
			
			
			
				
			
			/*
			Vector2f fst = p1->proj_pos;
			Vector2f snd = p2->proj_pos;
			Vector2f trd = p3->proj_pos;
			if (p1->pos != p1->proj_pos) {
				fst = this->jointer(p1,p2);
			}
			if (p2->pos != p2->proj_pos) {
				snd = this->jointer(p2,p3);
			}
			if (p3->pos != p3->proj_pos) {
				trd = this->jointer(p3,p2);
			}
			p1->proj_pos = fst;
			p2->proj_pos = snd;
			p3->proj_pos = trd;
			*/
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
		
		Point p4;
		Point p5;
		Joint holder;
		Joint holder2;
	
		Stick() : pinned(false) {}
		Stick(Vector2f pos, float length) :
			pinned(false),
			p1(Vector2f(pos.x-length,pos.y),1),
			p2(Vector2f(pos.x,pos.y),1),
			p3(Vector2f(pos.x+length,pos.y),1),
			p4(Vector2f(pos.x+length*2,pos.y),1),
			p5(Vector2f(pos.x+length*3,pos.y),1),
			holder(&this->p1, &this->p2, &this->p3, length,length, 0.5,false),
			holder2(&this->p3, &this->p4, &this->p5, length, length, 0.5, true)
		{
			this->type = O_STICK;
		}
	
	
		Stick& operator=(const Stick &rhs) {
			if (this == &rhs)
				return *this;
			this->p1 = rhs.p1;
			this->p2 = rhs.p2;
			this->p3 = rhs.p3;
			this->p4 = rhs.p4;
			this->p5 = rhs.p5;
			this->holder = rhs.holder;
			this->holder.p1 = &this->p1;
			this->holder.p2 = &this->p2;
			this->holder.p3 = &this->p3;
			
			this->holder2 = rhs.holder2;
			this->holder2.p1 = &this->p3;
			this->holder2.p2 = &this->p4;
			this->holder2.p3 = &this->p5;
			return *this;
		}
	
		void draw() {
			this->p1.draw();

			this->p2.draw();

			this->p3.draw();
			
			this->p4.draw();
			this->p5.draw();

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
			this->p4.was_computed = false;
			this->p5.was_computed = false;
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
			project_pt(&p1,ext,dt);
			project_pt(&p2,ext,dt);
			project_pt(&p3,ext,dt);
			project_pt(&p4,ext,dt);
			project_pt(&p5,ext,dt);
		}
		void update_pt(Point *p, float dt) {
			p->speed = (p->proj_pos - p->pos) / dt;
			p->pos = p->proj_pos;
		}
		void update(float dt) {
			//update_pt(&p1, dt);
			update_pt(&p2, dt);
			update_pt(&p3, dt);
			update_pt(&p4, dt);
			update_pt(&p5, dt);
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


	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,800, 600,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();

	
	World world;
	
	
	world.add_stick(Vector2f(200,200),50);
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
