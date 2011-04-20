#include "PhysTest.h"



class Point {
	public:
		Vector2f pos;
		Vector2f proj_pos;
		Vector2f speed;
		float inv_mass;
		bool was_computed;
		bool pinned;

		Point() : 
			pos(0,0), 
			proj_pos(0,0), 
			speed(0,0), 
			inv_mass(1), 
			was_computed(false),
			pinned(false)
		{}
		Point(Vector2f pos, float inv_mass, bool pinned) :
			pos(pos),
			proj_pos(pos),
			speed(0,0),
			inv_mass(inv_mass),
			was_computed(false),
			pinned(pinned)
		{}

		void draw() {
			glLoadIdentity();
			glTranslatef(pos.x, pos.y, 0 );
			
			glColor3f(1.0,1.0,1.0);
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
		float length1;
		bool upper;
		Joint *prev;
		Joint *next;

		Joint() : 
			p1(NULL), 
			p2(NULL), 
			stiffness(0), 
			length1(0), 
			upper(true),
			prev(NULL),
			next(NULL)
		{}
		Joint(Point *p1, Point *p2, float length1, float stiffness, bool upperm) :
			p1(p1),
			p2(p2),
			stiffness(stiffness),
			length1(length1),
			upper(upper),
			prev(NULL),
			next(NULL)
		{

		}
	
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
		void apply() {
			
			Vector2f delta = p2->proj_pos - p1->proj_pos;
			float deltalen = delta.length();
			float diff = (deltalen - length1)/deltalen;
			p1->proj_pos += delta * (0.5 * diff);
			p2->proj_pos -= delta * (0.5 * diff);

		}
	
};

enum ObjectType {
	O_INVALID,
	O_STICK,
	O_PLAIN,
	O_BRICK,
	O_LAST
};

class Brick;

class Object {
	public:
		bool pinned;
		Vector2f pos;
		Vector2f p;
		Vector2f F;
		Vector2f V;
		float H;
		float W;
		float angle;
		float width;
		float height;
		float mass;
		float inertia;
		
		float r,g,b;
	
		ObjectType type;
	
		virtual ~Object() {};
	
		virtual void draw() = 0;
		virtual void compute(Brick *) = 0;
		virtual void project(Vector2f, float) = 0;
		virtual void update(Vector2f,float) = 0;
		virtual void apply_impulse(Vector2f,Vector2f,float) = 0;
};

class Brick : public Object {
	public:
		Vector2f p1;
		Vector2f p2;
		Vector2f p3;
		Vector2f p4;
		


		Brick() {
		
		}
		
		Brick(Vector2f pos, float width, float height, bool pinned = false)
		{
			this->pinned = pinned;
			this->pos = pos;
			this->p = pos;
			this->F = Vector2f(0,0);
			this->V = Vector2f(0,0);
			this->H = 0;
			this->W = 0;
			this->angle = 0;
			this->width = width;
			this->height = height;
			float R = (width + height) / 2;
			mass = R *R * 0.1;
			inertia = 0.05 * mass * R * R;
			
			r=(((random)())%1000)*0.001;
			g=(((random)())%1000)*0.001;
			b=(((random)())%1000)*0.001;
			
			p1 = pos + Vector2f(-width/2,-height/2);
			p2 = pos + Vector2f(-width/2,height/2);
			p3 = pos + Vector2f(width/2,height/2);
			p4 = pos + Vector2f(width/2,-height/2);
			
		}


		void draw() {
			glLoadIdentity();
			glColor3f(r,g,b);
			glBegin(GL_QUADS);
				glVertex2f(p1.x,p1.y);
				glVertex2f(p2.x,p2.y);
				glVertex2f(p3.x,p3.y);
				glVertex2f(p4.x,p4.y);
			glEnd();
			
			glLoadIdentity();
			glTranslatef(p1.x, p1.y, 0 );
			
			glColor3f(1.0,0.0,0.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			
			glLoadIdentity();
			glTranslatef(p2.x, p2.y, 0 );
			
			glColor3f(0.0,1.0,0.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			
			glLoadIdentity();
			glTranslatef(p3.x, p3.y, 0 );
			
			glColor3f(0.0,0.0,1.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			
			glLoadIdentity();


		}

		void compute() {
		}
		void compute(Brick *obj) {
			Vector2f ADL = pos - Vector2f(width/2,0);
			Vector2f ADR = pos + Vector2f(width/2,0);
			
			Vector2f Res;
			Vector2f normal;
			Vector2f ResSpeed;
			
			Vector2f R1;
			Vector2f R2;
			normal = (Vector2f (0,-1) - V).normalize();
			float impulse;
			float Z1,Z2;
			float J;
			float Vab;
			if (lines_intersect(pos,p2,obj->p4,obj->p1,&Res)) {
				R1 = Res - pos;
				R2 = Res - obj->pos;
				this->apply_impulse(Res,normal,500);
			}
			if (lines_intersect(pos,p3,obj->p4,obj->p1,&Res)) {
				R1 = Res - pos;
				R2 = Res - obj->pos;
				this->apply_impulse(Res,normal,500);
			}
			/*
			if (lines_intersect(pos,p3,obj->p4,obj->p1,&Res)) {
				R1 = Res - pos;
				R2 = Res - obj->pos;
				Vab = 
				normal.x * (V.x - W * R1.y - obj->V.x + obj->W * R2.y) +
				normal.y * (V.y + W * R1.x - obj->V.y - obj->W * R2.x);
				Z1 = (normal.y * R1.x - normal.x * R1.y) * (1/inertia);
				Z2 = (normal.y * R2.x - normal.x * R2.y) * (1/inertia);
				J = normal.x * (normal.x * (1/mass) - R1.y * Z1 + normal.x * (1/obj->mass) + R2.y * Z2)
				 + normal.y * (normal.y * (1/mass) + R1.x * Z1 * (1/inertia) + normal.y * (1/obj->mass) - R2.x * Z2 * (1/obj->inertia));
				
				impulse = (0 - (1 + 1) * Vab) / J;
				printf("%f\n",impulse);
				if (impulse > 0)
				this->apply_impulse(Res,normal,impulse);
			}
			*/

			
			
		}
		void project(Vector2f ext, float dt) {
			
			p1 = Vector2f(-width/2,-height/2);
			p2 = Vector2f(-width/2,height/2);
			p3 = Vector2f(width/2,height/2);
			p4 = Vector2f(width/2,-height/2);			

			p1 = pos + rotate(p1,angle);
			p2 = pos + rotate(p2,angle);
			p3 = pos + rotate(p3,angle);
			p4 = pos + rotate(p4,angle);
			if (this->pinned) return;
			this->pos += this->V * dt;
			float diff = this->W * dt;
			this->angle += diff;

			
			this->V += (this->F+ext) / mass * dt;
			if (fabs((H / inertia) * dt) > 0.0001)
			W += (H / inertia) * dt;


		}
		void update(Vector2f ext, float dt) {

				
		}
		void apply_impulse(Vector2f place, Vector2f normal, float impulse) {
			Vector2f RA = place - pos;
			this->V += normal * impulse * (1/mass);
			this->W += impulse * (normal.y * RA.x - normal.x * RA.y) * (1/inertia);
		}
};


class World {
	public:
		Array<Brick> bricks;
		std::list<Object *> objects;

		Vector2f gravity;
		float dt;

		World() : gravity(0,9.8), dt(0.001) {}

		Object* add_brick(Vector2f pos, float width, float height, bool pinned = false) {
			size_t num;
			num = this->bricks.add_item(Brick(pos,width,height,pinned));
			this->objects.push_back((Object *)this->bricks.get_find_ref(num));
			return (Object *)this->bricks.get_find_ref(num);
		}

		void draw() {
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				(*it)->draw();
			}
		}
		void iterate() {
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				int start = SDL_GetTicks();
				while((SDL_GetTicks() - start) < (dt*1000))
					(*it)->project(gravity,dt);	
			}
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				for (std::list<Object *>::iterator ti = it; ti != this->objects.end(); ti++) {
					if (ti == it) ti++;
					if (ti == this->objects.end()) break;
					(*it)->compute((Brick *)(*ti));
					//(*ti)->apply_impulse(Vector2f(500,200),Vector2f(0,-100),0.1);
				}
			}
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
			}
		}
};

void test() {
	SDL_Event event;
    bool quit = false;
    		
		
		Vector2f init(300,300);
		Vector2f vec(-200,0);
			
		
		Vector2f A(200,100);
		Vector2f B(500,100);
		Vector2f C(490, 50);
		Vector2f D(600, 300);
		
		Vector2f P;
		
		lines_intersect(A,B,C,D,&P);
    
    while(quit == false) {
		SDL_PollEvent(&event);
		if(event.type == SDL_QUIT)
			quit = true;
			
		glClear( GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
		glLoadIdentity();

		//glTranslatef(300, 300, 0 );
		
		
		glTranslatef(P.x, P.y, 0 );
		glColor3f(1.0,0.0,0.0);
		glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				glVertex2f(10 * cos(i), 10 * sin(i));
			}
		glEnd();
		
		//glTranslatef(radius, radius, 0 );

		glLoadIdentity();
		glColor3f(1.0,0.0,0.0);
		glLineWidth(3);
		glBegin(GL_LINES);
			glVertex2f(A.x,A.y);
			glVertex2f(B.x,B.y);
		glEnd();
		
		glColor3f(0.0,1.0,0.0);
		glLineWidth(3);
		glBegin(GL_LINES);
			glVertex2f(C.x,C.y);
			glVertex2f(D.x,D.y);
		glEnd();

		
		
		SDL_GL_SwapBuffers();
	}

	
	
	exit(0);
}




int main(int argc, char **argv) {

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
	Object *br = world.add_brick(Vector2f(300,300),100,100);
	br->angle=0;

	
	Object *bl = world.add_brick(Vector2f(400,600),500,100,true);
	bl->angle = 0;
		
	
//	test();
	
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
