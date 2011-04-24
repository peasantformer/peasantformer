#include "PhysTest.h"



class Point {
	public:
		Vector2f pos;
		Vector2f prev_pos;
		Vector2f V;
		Vector2f F;
		float inv_mass;
		float mass;
		float inertia;
		bool was_computed;
		bool pinned;
		float W;
		float angle;
		float H;

		Point() : 
			pos(0,0), 
			V(0,0),
			F(0,0),
			inv_mass(1), 
			mass(1/inv_mass),
			was_computed(false),
			pinned(false),
			W(0)
		{
			float R = (100) / 2;
			mass = R *R * 0.005;
			inertia = mass * R * R * 0.01;
		}
		Point(Vector2f pos, float inv_mass, bool pinned) :
			pos(pos),
			prev_pos(pos),
			V(0,0),
			F(0,0),
			inv_mass(inv_mass),
			mass(1/inv_mass),
			was_computed(false),
			pinned(pinned),
			W(0)
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
		
		void project(Vector2f ext, float dt) {
			if (this->pinned) return;
			this->prev_pos = pos;
			this->pos += this->V * dt;
			this->angle += W *dt;

			this->V += (this->F) / mass * dt + ext * dt;
			if (fabs((H / inertia) * dt) > 0.0001)
			W += (H / inertia) * dt;
		
		}
		
		void apply_impulse(Vector2f place, Vector2f normal, float impulse) {
			if (this->pinned) return;
			if (impulse > 0.0001) {
				printf("%f\n",impulse);
				Vector2f RA = place - pos;
				V += normal * impulse * (1/mass);
				W += impulse * (normal.y * RA.x - normal.x * RA.y) * (1/inertia);
			}
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
		Vector2f prev_pos;
		Vector2f p;
		Vector2f F;
		Vector2f V;
		float H;
		float W;
		float angle;
		float prev_angle;
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
		//virtual void apply_impulse(Vector2f,Vector2f,Vector2f) = 0;
};
		Vector2f R1;
		Vector2f R2;
		
class LinearJoint {
	public:
		Point *p1;
		Point *p2;
		float length;
		float lenmin;
		float lenmax;
		LinearJoint * next;
		LinearJoint * prev;
		LinearJoint() {
			this->p1 = NULL;
			this->p2 = NULL;
			this->length = 0;
			this->next = NULL;
			this->prev = NULL;
		}
		LinearJoint(Point *p1, Point *p2, float length)  {
			this->p1 = p1;
			this->p2 = p2;
			this->length = length;
			this->lenmin = length - 10;
			this->lenmax = length + 10;
			this->next = NULL;
			this->prev = NULL;
		}
		
		void draw() {
			glLoadIdentity();
			glColor3f(1.0,0.0,0.0);
			glLineWidth(10);
			glBegin(GL_LINES);
				glVertex2f(p1->pos.x,p1->pos.y);
				glVertex2f(p2->pos.x,p2->pos.y);
 			glEnd();
		}
		void apply() {
			Vector2f normal_s, normal_r, normal;
			Vector2f s,r;
			float impulse_s, impulse_r, impulse;
			float m = 10;
			float desired_length;
			if (p1->was_computed == false) {
				normal = (p1->pos - p2->pos).normalize();
				impulse = ((p1->pos - p2->pos).length() - length) * p1->V.length() * p1->mass * m;
				p1->apply_impulse(p1->pos,normal,impulse);
				p1->apply_impulse(p1->pos,normal *-1,impulse * -1);
				p1->was_computed = true;
			}
				
			//if (p2->was_computed == false) {
				normal = (p1->pos - p2->pos).normalize();
				//impulse = ((p1->pos - p2->pos).length() - length) * p2->V.length() * p2->mass * m;
				float r1 =  (p2->pos-p1->pos).length();
				float r2 =  (p2->prev_pos-p1->prev_pos).length();
				//impulse_s = (r1 - length) * m;

					
				p2->F = normal * ((r1-length));
				//printf("%f\n",r1);
				
				//p2->apply_impulse(p2->pos,normal,impulse_s);
				//p2->apply_impulse(p2->pos,normal * -1,impulse_s * -1);
				//impulse_r = fabs(r1 - r2) - length;
				printf(">> %f\n",p2->F.length());
				//p2->apply_impulse(p2->pos,normal,impulse_r);
				p2->was_computed = true;
			//}
								
				
				
				
				
				
				//impulse = ps / (1/p2->mass);
				
				//ps / (1/mass) / normal  = impulse;
				//impulse = ps / (1/p2->mass);
				//printf("%f\n",normal.y);
				//p2->pos += normal * ps * 0.0001 * (1/p2->mass);

				


		

		
			
			/*
			float len = length;
			
			for (LinearJoint *nxt = this->next; nxt != NULL; nxt = nxt->next) {
				len += nxt->length;
				normal = (p1->pos - nxt->p2->pos).normalize();
				
				imp_stright = (p1->pos - nxt->p2->pos).length() - len;
				imp_reverse = len - (nxt->p2->pos - p1->pos).length();
				
				nxt->p2->apply_impulse(nxt->p2->pos,normal,imp_stright);
				nxt->p2->apply_impulse(nxt->p2->pos,normal * -1,imp_reverse);
			}
			*/
		}
};
		
class Brick : public Object {
	public:
		Point p1;
		Point p2;
		Point p3;
		Point p4;
		
		LinearJoint lj1;
		LinearJoint lj2;
		LinearJoint lj3;
		
		Brick() {
		}
		
		Brick& operator=(const Brick &rhs) {
			if (this == &rhs)
				return *this;
			this->p1 = rhs.p1;
			this->p2 = rhs.p2;
			this->p3 = rhs.p3;
			this->p4 = rhs.p4;
			
			this->lj1 = rhs.lj1;
			this->lj2 = rhs.lj2;
			this->lj3 = rhs.lj3;
			
			this->lj1.next = &lj2;
			this->lj2.next = &lj3;
			
			this->lj3.prev = &lj2;
			this->lj2.prev = &lj1;
			
			this->lj1.p1 = &p1;
			this->lj1.p2 = &p2;
			
			this->lj2.p1 = &p2;
			this->lj2.p2 = &p3;
			
			this->lj3.p1 = &p3;
			this->lj3.p2 = &p4;
			
			return *this;
		}
		
		Brick(Vector2f pos, float length) {
			this->pos = pos;
			this->p1 = Point(pos,1,true);
			this->p2 = Point(pos+Vector2f(length,0),1,false);
			this->p3 = Point(pos+Vector2f(length*2,0),1,false);
			this->p4 = Point(pos+Vector2f(length*3,0),1,false);
			
			this->lj1 = LinearJoint(&p1,&p2,length);
			this->lj2 = LinearJoint(&p2,&p3,length);
			this->lj3 = LinearJoint(&p3,&p4,length);
			
			lj1.next = &lj2;
			lj2.next = &lj3;
			
			lj3.prev = &lj2;
			lj2.prev = &lj1;
		}
		virtual void draw() {
			this->p1.draw();
			this->p2.draw();
			//this->p3.draw();
			//this->p4.draw();
			
			this->lj1.draw();
			//this->lj2.draw();
			//this->lj3.draw();
		}
		virtual void compute(Brick *) {
			this->p1.was_computed = false;
			this->p2.was_computed = false;
			//this->p3.was_computed = false;
			//this->p4.was_computed = false;
			
			this->lj1.apply();
			//this->lj2.apply();
			//this->lj3.apply();
		}
		virtual void project(Vector2f ext, float dt) {
			p1.project(ext,dt);
			p2.project(ext,dt);
			//p3.project(ext,dt);
			//p4.project(ext,dt);
		}
		virtual void update(Vector2f,float) {
		}
	
};
/*
class Brick : public Object {
	public:
		Vector2f p1;
		Vector2f p2;
		Vector2f p3;
		Vector2f p4;
		

	
		Brick() {
		}
		
		Brick(Vector2f pos, float width, float height, bool pinned = false) {
			this->pos = pos;
			this->prev_pos = pos;
			this->width = width;
			this->height = height;
			this->pinned = pinned;
			this->angle = 0;
			this->prev_angle = angle;
			this->W = 0;
			
			r=(((random)())%1000)*0.001;
			g=(((random)())%1000)*0.001;
			b=(((random)())%1000)*0.001;
			
			float R = (width + height) / 2;
			mass = R *R * 0.05;
			inertia = mass * R * R * 0.01;
			
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
			glTranslatef(pos.x, pos.y, 0 );
			
			glColor3f(1.0,1.0,1.0);
			glBegin(GL_POLYGON);
				for (int i=0; i <= 360; i++) {
					glVertex2f(10 * cos(i), 10 * sin(i));
				}
				
			glEnd();
			

		}
		void apply_impulse(Vector2f place, Vector2f normal, float impulse) {
			if (this->pinned) return;
			if (impulse > 0.0001) {
				printf("%f\n",impulse);
				Vector2f RA = place - pos;
				V += normal * impulse * (1/mass);
				W += impulse * (normal.y * RA.x - normal.x * RA.y) * (1/inertia);
			}
		}
		float calculate_impulse(Vector2f Res, Vector2f normal, Brick *obj) {
			Vector2f R1 = Res - pos;
			Vector2f R2 = Res - obj->pos;
			float Vab = 
				normal.x * (V.x - W*0 * R1.y - obj->V.x + obj->W*0 * R2.y) +
				normal.y * (V.y + W*0 * R1.x - obj->V.y - obj->W*0 * R2.x);
				

				
			
			float Z1 = (normal.y * R1.x - normal.x * R1.y) * (1/inertia);
			float Z2 = (normal.y * R2.x - normal.x * R2.y) * (1/obj->inertia);
			
			float J = normal.x *  (normal.x * (1/mass) - R1.y * Z1 + normal.x * (1/obj->mass) + R2.y * Z2) 
			+ normal.y * (normal.y * (1/mass) + R1.x * Z1 * (1/inertia)
			+ normal.y * (1/obj->mass) - R2.x * Z2 * (1/obj->inertia));
			
			return (0 - (1 + 0.6) * Vab) / J;
			
		}
		void compute(Brick *obj) {
			
			Vector2f Res;
			Vector2f normal(0,0);
			float impulse = 0;
			bool was = false;
			
			normal = ((prev_pos - pos)).normalize();
						
			
			if (
				   lines_intersect(pos,p1,obj->p1,obj->p2,&Res)
				|| lines_intersect(pos,p1,obj->p2,obj->p3,&Res)
				|| lines_intersect(pos,p1,obj->p3,obj->p4,&Res)
				|| lines_intersect(pos,p1,obj->p1,obj->p4,&Res)
			) {
				impulse = calculate_impulse(Res, normal, obj);
				apply_impulse(Res,normal,impulse);
				obj->apply_impulse(Res,normal,-impulse);
				this->pos += (Res-p1);
				this->prev_pos = pos;
				was = true;
			}
			
			
			if (
				   lines_intersect(pos,p2,obj->p1,obj->p2,&Res)
				|| lines_intersect(pos,p2,obj->p2,obj->p3,&Res)
				|| lines_intersect(pos,p2,obj->p3,obj->p4,&Res)
				|| lines_intersect(pos,p2,obj->p1,obj->p4,&Res)
			) {
				impulse = calculate_impulse(Res, normal, obj);
				apply_impulse(Res,normal,impulse);
				obj->apply_impulse(Res,normal,-impulse);
				this->pos += (Res-p2);
				this->prev_pos = pos;
				was = true;
			}
			
			
			
			if (
				   lines_intersect(pos,p3,obj->p1,obj->p2,&Res)
				|| lines_intersect(pos,p3,obj->p2,obj->p3,&Res)
				|| lines_intersect(pos,p3,obj->p3,obj->p4,&Res)
				|| lines_intersect(pos,p3,obj->p1,obj->p4,&Res)
			) {
				impulse = calculate_impulse(Res, normal, obj);
				apply_impulse(Res,normal,impulse);
				obj->apply_impulse(Res,normal,-impulse);
				this->pos += (Res-p3);
				this->prev_pos = pos;
				was = true;
			}
			
			
			
			if (
				   lines_intersect(pos,p4,obj->p1,obj->p2,&Res)
				|| lines_intersect(pos,p4,obj->p2,obj->p3,&Res)
				|| lines_intersect(pos,p4,obj->p3,obj->p4,&Res)
				|| lines_intersect(pos,p4,obj->p1,obj->p4,&Res)
			) {
				impulse = calculate_impulse(Res, normal, obj);
				apply_impulse(Res,normal,impulse);
				obj->apply_impulse(Res,normal,-impulse);
				this->pos += (Res-p4);
				this->prev_pos = pos;
				was = true;
			}
			
			
			if (was == true) {
				W /= 10;
				//V -= V;
			}

			
			
			
		}
		void project(Vector2f ext, float dt) {

		
			prev_pos = pos;
			prev_angle = angle;

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
			this->angle += W *dt;

			this->V += (this->F) / mass * dt + ext * dt;
			if (fabs((H / inertia) * dt) > 0.0001)
			W += (H / inertia) * dt;
			
		}
		void update(Vector2f ext,float dt) {

		}
		//void apply_impulse(Vector2f,Vector2f,Vector2f) {
		//}
};
*/

class World {
	public:
		Array<Brick> bricks;
		std::list<Object *> objects;

		Vector2f gravity;
		float dt;

		World() : gravity(0,0.098), dt(0.0001) {}

		Object* add_brick(Vector2f pos, float length) {
			size_t num;
			num = this->bricks.add_item(Brick(pos,length));
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
				int start = SDL_GetTicks();
				while((SDL_GetTicks() - start) < (dt*1000))
					(*it)->project(gravity,dt);	
			}
			for (std::list<Object *>::iterator it = this->objects.begin(); it != this->objects.end(); it++) {
				for (std::list<Object *>::iterator ti = it; ti != this->objects.end(); ti++) {
					//if (ti == it) ti++;
					//if (ti == this->objects.end()) break;
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
		Vector2f C(590, 50);
		Vector2f D(600, 300);
		
		Vector2f P;
		
		printf("%d\n",lines_intersect(A,B,C,D,&P));
    
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
	Object *br = world.add_brick(Vector2f(300,100),100);
	//br->angle=-10;
	
	//world.add_brick(Vector2f(300,00),50,50);


	
	//Object *bl = world.add_brick(Vector2f(400,300),500,50,true);
	//bl->angle = 00;
	

	
		
	
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
