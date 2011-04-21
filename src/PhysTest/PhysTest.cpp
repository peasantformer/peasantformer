#include "PhysTest.h"



class Point {
	public:
		Vector2f pos;
		Vector2f proj_pos;
		Vector2f speed;
		Vector2f speedmod;
		float inv_mass;
		float mass;
		bool was_computed;
		bool pinned;

		Point() : 
			pos(0,0), 
			proj_pos(0,0), 
			speed(0,0),
			speedmod(0,0), 
			inv_mass(1), 
			mass(1/inv_mass),
			was_computed(false),
			pinned(false)
		{}
		Point(Vector2f pos, float inv_mass, bool pinned) :
			pos(pos),
			proj_pos(pos),
			speed(0,0),
			speedmod(0,0),
			inv_mass(inv_mass),
			mass(1/inv_mass),
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
		Vector2f speed;
		Vector2f speedmod;
		float angle;
		float width;
		float height;
		Vector2f pos;
		Vector2f proj_pos;
		
		float r,g,b;
	
		ObjectType type;
	
		virtual ~Object() {};
	
		virtual void draw() = 0;
		virtual void compute(Brick *) = 0;
		virtual void project(Vector2f, float) = 0;
		virtual void update(Vector2f,float) = 0;
		//virtual void apply_impulse(Vector2f,Vector2f,Vector2f) = 0;
};
		Vector2f R1;
		Vector2f R2;
class Brick : public Object {
	public:
		Point p1;
		Point p2;
		Point p3;
		Point p4;
		

	
		Brick() {
		}
		
		Brick(Vector2f pos, float width, float height, bool pinned = false) {
			this->pos = pos;
			this->proj_pos = pos;
			this->width = width;
			this->height = height;
			this->pinned = pinned;
			this->angle = 0;
			this->speed = Vector2f(0,0);
			
			r=(((random)())%1000)*0.001;
			g=(((random)())%1000)*0.001;
			b=(((random)())%1000)*0.001;
			
			p1 = Point(pos + Vector2f(-width/2,-height/2),1,this->pinned);
			p2 = Point(pos + Vector2f(-width/2,height/2),1,this->pinned);
			p3 = Point(pos + Vector2f(width/2,height/2),1,this->pinned);
			p4 = Point(pos + Vector2f(width/2,-height/2),1,this->pinned);
		}
		
		void draw() {
			glLoadIdentity();
			glColor3f(r,g,b);
			glBegin(GL_QUADS);
				glVertex2f(p1.pos.x,p1.pos.y);
				glVertex2f(p2.pos.x,p2.pos.y);
				glVertex2f(p3.pos.x,p3.pos.y);
				glVertex2f(p4.pos.x,p4.pos.y);
			glEnd();
			
			glLoadIdentity();
			glTranslatef(p1.pos.x, p1.pos.y, 0 );
			
			glColor3f(1.0,0.0,0.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			
			glLoadIdentity();
			glTranslatef(p2.pos.x, p2.pos.y, 0 );
			
			glColor3f(0.0,1.0,0.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			
			glLoadIdentity();
			glTranslatef(p3.pos.x, p3.pos.y, 0 );
			
			glColor3f(0.0,0.0,1.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			
			glLoadIdentity();
			glTranslatef(pos.x, pos.y, 0 );
			
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			

		}
		void apply_impulse(Vector2f place, Vector2f normal, float impulse) {
			Vector2f RA = place - pos;
			
			this->speedmod = normal * impulse * 5;
			 
			/*
			p1.speedmod = normal * impulse * p1.inv_mass;
			p2.speedmod = normal * impulse * p2.inv_mass;
			p3.speedmod = normal * impulse * p3.inv_mass;
			p4.speedmod = normal * impulse * p4.inv_mass;
			*/
			
			
			
			
			//p1.speedmod 
		}
		void compute(Brick *obj) {
			
			Vector2f Res;
			Vector2f normal;
			
			if (lines_intersect(pos,p2.proj_pos,obj->p4.proj_pos,obj->p1.proj_pos,&Res)) {
				R1 = p2.proj_pos;
				R2 = Res;
				R2.x = R1.x + (R2.x - R1.x);
				R2.y = R1.y + (R2.y - R1.y);
				
				normal = (R2-R1).normalize();
				normal = Vector2f(0,-1);
				apply_impulse(Res,normal,(Res-p2.proj_pos).length() * 0.5);
				//p2.proj_pos += Vector2f(0,-1) * (R2-R1).length();
				//SDL_Delay(100);
				
			}
			
			
			if (lines_intersect(pos,pos+Vector2f(0,height/2),obj->p4.proj_pos,obj->p1.proj_pos,&Res)) {
				
				R1 = p3.proj_pos;
				R2 = Res;
				R2.x = R1.x + (R2.x - R1.x) * 10;
				R2.y = R1.y + (R2.y - R1.y) * 10;
				
				normal = (R2-R1).normalize();
				normal = Vector2f(0,-1);
				apply_impulse(Res,normal,((pos+Vector2f(0,height/2))-Res).length());
				//p3.proj_pos +=  Vector2f(0,-1) * (R2-R1).length();
				
			}
			
			
		}
		void project(Vector2f ext, float dt) {
			if (this->pinned == true) return;
			this->speed += ext *dt / 1 + speedmod / dt;
			/*
			p1.speed += ext * dt / p1.inv_mass;
			p2.speed += ext * dt / p2.inv_mass;
			p3.speed += ext * dt / p3.inv_mass;
			p4.speed += ext * dt / p4.inv_mass;	
			*/
			/*
			Vector2f Xcm =
				p1.pos * p1.mass +
				p2.pos * p2.mass + 
				p3.pos * p3.mass + 
				p4.pos * p4.mass;
			Vector2f Vcm =
				p1.speed * p1.mass + 
				p2.speed * p2.mass + 
				p3.speed * p3.mass + 
				p4.speed * p4.mass;
				

			
			Vector2f r1 = p1.pos - Xcm;
			Vector2f r2 = p2.pos - Xcm;
			Vector2f r3 = p3.pos - Xcm;
			Vector2f r4 = p4.pos - Xcm;
			
			Vector2f L = 
				r1 * (p1.speed * p1.mass) +
				r2 * (p2.speed * p2.mass) +
				r3 * (p3.speed * p3.mass) +
				r4 * (p4.speed * p4.mass);
				
			float A11 =
				p1.pos.x * p1.pos.x * p1.mass +
				p2.pos.x * p2.pos.x * p2.mass +
				p3.pos.x * p3.pos.x * p3.mass +
				p4.pos.x * p4.pos.x * p4.mass;
			float A12 = -1 * (
				p1.pos.x * p1.pos.y * p1.mass +
				p2.pos.x * p2.pos.y * p2.mass +
				p3.pos.x * p3.pos.y * p3.mass +
				p4.pos.x * p4.pos.y * p4.mass
			);
			
			float A21 = A12;
			
			float A22 = 
				p1.pos.y * p1.pos.y * p1.mass +
				p2.pos.y * p2.pos.y * p2.mass +
				p3.pos.y * p3.pos.y * p3.mass +
				p4.pos.y * p4.pos.y * p4.mass;
			
			float Adet = (A11 * A22) - (A12 * A21);
			
			float IAdet = 1 / Adet;
			
			Vector2f R;
			
			R.x = (A22 * IAdet * L.x) - (A12 * IAdet * L.y);
            R.y = (A11 * IAdet * L.y) - (A21 * IAdet * L.x);


            
            Vector2f dv1 = Vcm + R * r1 - p1.speed;
            Vector2f dv2 = Vcm + R * r2 - p2.speed;
            Vector2f dv3 = Vcm + R * r3 - p3.speed;
            Vector2f dv4 = Vcm + R * r4 - p4.speed;
            

            p1.speed += dv1;
            p2.speed += dv2;
            p3.speed += dv3;
            p4.speed += dv4;
            */
            
				


          /*
			p1.proj_pos = p1.pos + p1.speed * dt;
			p2.proj_pos = p2.pos + p2.speed * dt;
			p3.proj_pos = p3.pos + p3.speed * dt;
			p4.proj_pos = p4.pos + p4.speed * dt;
			*/
			this->proj_pos = this->pos + this->speed * dt;
				
			
		}
		void update(Vector2f ext,float dt) {
			if (this->pinned == true) return;	
			
			this->speed = (this->proj_pos - this->pos) / dt;
			this->pos = proj_pos;
			p1.pos = pos + Vector2f(-width/2,-height/2);
			p2.pos = pos + Vector2f(-width/2,height/2);
			p3.pos = pos + Vector2f(width/2,height/2);
			p4.pos = pos + Vector2f(width/2,-height/2);
			/*
			p1.speed = (p1.proj_pos - p1.pos) / dt + p1.speedmod;
			p2.speed = (p2.proj_pos - p2.pos) / dt + p2.speedmod;
			p3.speed = (p3.proj_pos - p3.pos) / dt + p3.speedmod;
			p4.speed = (p4.proj_pos - p4.pos) / dt + p4.speedmod;
			
			p1.pos = p1.proj_pos;
			p2.pos = p2.proj_pos;
			p3.pos = p3.proj_pos;
			p4.pos = p4.proj_pos;
			* 			lines_intersect(p1.pos,p3.pos,p2.pos,p4.pos,&pos);
			*/

		}
		//void apply_impulse(Vector2f,Vector2f,Vector2f) {
		//}
};


class World {
	public:
		Array<Brick> bricks;
		std::list<Object *> objects;

		Vector2f gravity;
		float dt;

		World() : gravity(0,0.98), dt(0.01) {}

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
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);
			glLineWidth(10);
			glBegin(GL_LINES);
				glVertex2f(R1.x,R1.y);
				glVertex2f(R2.x,R2.y);
 			glEnd();
			glLoadIdentity();	
		}
		void iterate() {
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
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
				(*it)->update(gravity,dt);	
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
	Object *br = world.add_brick(Vector2f(300,100),50,50);
	br->angle=45;

	
	Object *bl = world.add_brick(Vector2f(400,300),500,50,true);
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
