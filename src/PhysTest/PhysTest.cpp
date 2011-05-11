#include "PhysTest.h"

Vector2f R1,R2;
Vector2f R3,R4;
Vector2f R5,R6;

class Point {
	public:
		Vector2f pos;
		Vector2f proj_pos;
		Vector2f prev_pos;
		Vector2f V;
		Vector2f F;
		float mass;
		bool pinned;
	public:
		Point() {
		}
		Point(Vector2f pos, float mass, bool pinned) {
			this->pos = pos;
			this->proj_pos = pos;
			this->prev_pos = pos;
			this->mass = mass;
			this->pinned = pinned;
		}
	public:
		void project(Vector2f ext, float dt) {
			if (pinned) {
				this->V = Vector2f(0,0);
				this->F = Vector2f(0,0);
				this->proj_pos = pos;
				return;
			}
			this->proj_pos = pos;
			this->V += ext * dt;
			this->proj_pos += V * dt;
		}
		void correct(float dt) {
			if (pinned) return;
			this->proj_pos = pos;
			this->V += F / (mass * dt);
			this->F = Vector2f(0,0);
			this->proj_pos += V * dt;
		}
		void apply(float dt) {
			this->prev_pos = pos;
			this->pos += V * dt;
		}
};

class Triangle {
	public:
		Vector2f pos;
		Vector2f V;          //         BB         // r - incircle radius
		Point A,B,C;         //         /\         // R - excircle radius
		float a,b,c;         //        /VB\        // P - perimeter
		float VA,VB,VC;      //       /    \       // p = P/2
		float r,R;           //     c/      \a     // A,B,C - vertices
		float p,P;           //     /        \     // VA,VB,VC - angles
		float angle;         //  A /_VA____VC_\ C  // a,b,c - sides:
		float mass;          //         bb         // V - triangle's speed
		float inertia;
		float W;             // angular speed
		bool pinned;
		Vector2f rot_center; // rotation center

	public:
		Triangle() {
		}
		Triangle(Vector2f pos, float a, float b, float c, bool pinned, float angle) {
			this->mass = 10;
			this->inertia = mass * mass * mass;
			this->pos = pos;
			this->rot_center = pos;
			this->a = a;
			this->b = b;
			this->c = c;
			this->angle = angle;

			this->P = a + b + c;
			this->p = P/2;

			this->R = a*b*c / sqrt((a+b+c) * (-a+b+c) * (a-b+c) * (a+b-c));
			this->r = sqrt(((p-a) * (p-b) * (p-c)) / p);

			this->VC = acos((a*a + b*b - c*c)/(2*a*b));
			this->VB = acos((a*a + c*c - b*b)/(2*a*c));
			this->VA = acos((c*c + b*b - a*a)/(2*c*b));


			float Lc = sqrt(4*a*b*p*(p-c))/(a+b);
			float Lb = sqrt(4*a*c*p*(p-b))/(a+c);
			float La = sqrt(4*c*b*p*(p-a))/(c+b);
			//float Lc = 2*a*b*cos(VC/2)/(a+b);
			//float Lb = 2*a*c*cos(VB/2)/(a+c);
			//float La = 2*c*b*cos(VA/2)/(c+b);
			
			//float cLB = Lc*2/3;
			//float cLA = La*2/3;
			//float cLC = Lc*2/3;

			//float cLC = sqrt(r*r + p-c);
			//float cLA = sqrt(r*r + p-a);
			//float cLB = sqrt(r*r + p-b);

			//working:
			//float cLC = r/sin(VC/2);
			//float cLB = r/sin(VB/2);
			//float cLA = r/sin(VA/2);
			
			float cLC = sqrt((p-c)*(p-c) + r*r);
			float cLB = sqrt((p-b)*(p-b) + r*r);
			float cLA = sqrt((p-a)*(p-a) + r*r);

			this->B = Point(pos + rotate(Vector2f(0,-cLB),angle),mass/3.0,pinned);
			this->A = Point(
				pos + 
				rotate(Vector2f(0,-cLA),angle - ( 180*(M_PI/180) - (VB/2 + VA/2))),
				mass/3.0,pinned
			);
			this->C = Point(
				pos + 
				rotate(Vector2f(0,-cLC),angle + ( 180*(M_PI/180) - (VB/2 + VC/2))),
				mass/3.0,pinned
			);
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
			int i = 0;
			do {
				Vector2f AB = (A.proj_pos - B.proj_pos);
				Vector2f BC = (B.proj_pos - C.proj_pos);
				Vector2f CA = (C.proj_pos - A.proj_pos);

				float real_a = BC.length();
				float real_b = CA.length();
				float real_c = AB.length();

				float a_diff = real_a - a;
				float b_diff = real_b - b;
				float c_diff = real_c - c;
				
				if (fabs(b_diff) > 0.0) {
					this->A.F += CA.normalize() * b_diff /2 ;
					this->C.F -= CA.normalize() * b_diff /2 ;
				}
				if (fabs(c_diff) > 0.0) {
					this->A.F -= AB.normalize() * c_diff /2 ;
					this->B.F += AB.normalize() * c_diff /2 ;
				}
				if (fabs(a_diff) > 0.0) {
					this->B.F -= BC.normalize() * a_diff /2 ;
					this->C.F += BC.normalize() * a_diff /2 ;
				}

				this->A.correct(dt);
				this->B.correct(dt);
				this->C.correct(dt);


				real_a = (B.proj_pos - C.proj_pos).length();
				real_b = (C.proj_pos - A.proj_pos).length();
				real_c = (A.proj_pos - B.proj_pos).length();

				real_p = (real_a + real_b + real_c)/2;

				realdiff = 0;
				realdiff += fabs(real_a - a);
				realdiff += fabs(real_b - b);
				realdiff += fabs(real_c - c);
				i++;
			} while(fabs(real_p - p) > 1 || realdiff > 1);
		}
		void update(float dt) {
			Vector2f prev = A.pos;

			this->A.apply(dt);
			this->B.apply(dt);
			this->C.apply(dt);

			Vector2f LA = A.pos + rotate((A.pos - C.pos),-VA/2);
			Vector2f LC = C.pos + rotate((C.pos - A.pos),VC/2);
			lines_intersect(A.pos,LA,C.pos,LC,&this->pos);

			if (p < 500) {
			R1 = A.pos;
			R2 = A.pos + A.V * 100;
			}
			
			Vector2f LAV = A.pos + Vector2f(-A.V.y,A.V.x);
			Vector2f LBV = B.pos + Vector2f(-B.V.y,B.V.x);
			

			if (lines_paralell(A.pos,LAV,B.pos,LBV)) {
				this->W = 0;
			} else {
				lines_intersect(A.pos,LAV,B.pos,LBV,&this->rot_center);
				Vector2f rv = rot_center - A.pos;
				this->W = A.V.length() * sin(fullAngleOfVector(rv,A.V)) / rv.length();
			}

			if (fabs(W) < 0.0001) {
				this->V = A.V;
			} else {
				this->V = ((rot_center - pos) * W);
			}
		}
		void apply_impulse(Vector2f place, Vector2f normal, float amount) {
			float bounceness = 0.001;
			
			Vector2f PP = pos - place;

			Vector2f PA = pos - A.pos;
			Vector2f PB = pos - B.pos;
			Vector2f PC = pos - C.pos;
			
			float CA = (PP - PA).length();
			float CB = (PP - PB).length();
			float CC = (PP - PC).length();
			
			//printf("B: %f %f %f\n",CA,CB,CC);
			
			normalize_three(&CA,&CB,&CC);

			CC = 1 - CC;
			CB = 1 - CB;
			CA = 1 - CA;
			
			//printf("A: %f %f %f\n",CA,CB,CC);
			//

			Vector2f RA = place - pos;

			float wdiff = (normal.y * RA.x - normal.x * RA.y) / inertia;

			printf(">>> %f\n",wdiff);

			Vector2f PA_norm = Vector2f(PA.y,-PA.x).normalize();
			Vector2f PB_norm = Vector2f(PB.y,-PB.x).normalize();
			Vector2f PC_norm = Vector2f(PC.y,-PC.x).normalize();

			A.V += normal * (amount / mass) * bounceness;
			B.V += normal * (amount / mass) * bounceness;
			C.V += normal * (amount / mass) * bounceness;

			A.V += PA_norm * wdiff / 3;
			B.V += PB_norm * wdiff / 3;
			C.V += PC_norm * wdiff / 3;


		}
		float calculate_impulse(Vector2f Res, Vector2f normal, Triangle *obj) {
			//float imp = ((PT.V + obj->V) * (obj->mass + mass)).length(); 

			float imp = (V * mass).length();
			printf("%f %f\n",imp, V.length());
			return imp;
		}
		void solve(Triangle *ti) {
			Vector2f Res;
			Vector2f normal;
			Vector2f diff;
			float imp;
			
			Vector2f ACB = A.pos - (A.pos - B.pos) / 2;
			Vector2f BCC = B.pos - (B.pos - C.pos) / 2;
			Vector2f CCA = C.pos - (C.pos - A.pos) / 2;
			
			Vector2f PACB = A.prev_pos - (A.prev_pos - B.prev_pos) / 2;
			Vector2f PBCC = B.prev_pos - (B.prev_pos - C.prev_pos) / 2;
			Vector2f PCCA = C.prev_pos - (C.prev_pos - A.prev_pos) / 2;
	
			
			if (lines_intersect(pos,CCA,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,CCA,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,CCA,ti->B.pos,ti->C.pos,&Res)
			) {	
				normal = V.normalize() * -1;
//				normal = (PCCA - CCA);
				diff = (Res - CCA);
				A.V += diff;
				B.V += diff;
				C.V += diff;
//				A.pos += diff;
//				B.pos += diff;
//				C.pos += diff;
				imp = calculate_impulse(Res,normal,ti);
				apply_impulse(Res,normal,imp);
			}	
			if (lines_intersect(pos,BCC,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,BCC,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,BCC,ti->B.pos,ti->C.pos,&Res)
			) {	
				normal = V.normalize() * -1;
//				normal = (PBCC - BCC);
				diff = (Res - BCC);
				A.V += diff;
				B.V += diff;
				C.V += diff;
//				A.pos += diff;
//				B.pos += diff;
//				C.pos += diff;
				imp = calculate_impulse(Res,normal,ti);
				apply_impulse(Res,normal,imp);
			}	
			if (lines_intersect(pos,ACB,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,ACB,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,ACB,ti->B.pos,ti->C.pos,&Res)
			) {	
				normal = V.normalize() * -1;
//				normal = (PACB - ACB);
				diff = (Res - ACB);
				A.V += diff;
				B.V += diff;
				C.V += diff;
//				A.pos += diff;
//				B.pos += diff;
//				C.pos += diff;
				imp = calculate_impulse(Res,normal,ti);
				apply_impulse(Res,normal,imp);
			}
			
			if (lines_intersect(pos,A.pos,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,A.pos,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,A.pos,ti->B.pos,ti->C.pos,&Res)
			) {
				normal = V.normalize() * -1;
//				normal = (A.prev_pos - A.pos);
				Vector2f diff = (A.prev_pos - A.pos);
				A.V += diff;
				B.V += diff;
				C.V += diff;
//				B.pos += diff;
//				C.pos += diff;
//				A.pos += diff;;
				imp = calculate_impulse(Res,normal,ti);
				apply_impulse(Res,normal,imp);
			}
			if (lines_intersect(pos,B.pos,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,B.pos,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,B.pos,ti->B.pos,ti->C.pos,&Res)
			) {
				normal = V.normalize() * -1;
//				normal = (B.prev_pos - B.pos);
				Vector2f diff = (B.prev_pos - B.pos);
				A.V += diff;
				B.V += diff;
				C.V += diff;
//				B.pos += diff;
//				C.pos += diff;
//				A.pos += diff;;
				imp = calculate_impulse(Res,normal,ti);
				apply_impulse(Res,normal,imp);
			}
			if (lines_intersect(pos,C.pos,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,C.pos,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,C.pos,ti->B.pos,ti->C.pos,&Res)
			) {
				normal = V.normalize() * -1;
//				normal = (C.prev_pos - C.pos);
				Vector2f diff = (C.prev_pos - C.pos);
				A.V += diff;
				B.V += diff;
				C.V += diff;
//				B.pos += diff;
//				C.pos += diff;
//				A.pos += diff;;
				imp = calculate_impulse(Res,normal,ti);
				apply_impulse(Res,normal,imp);
			}
			
			/*
			if (lines_intersect(pos,A.pos,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,A.pos,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,A.pos,ti->B.pos,ti->C.pos,&Res)
			) {
				normal = (A.prev_pos-A.pos).normalize();
				Vector2f diff = (Res - A.pos);
				B.pos += diff;
				C.pos += diff;
				A.pos += diff;;
				imp = calculate_impulse(A,Res,ti);
				apply_impulse(Res,normal,imp);
			}
			if (lines_intersect(pos,B.pos,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,B.pos,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,B.pos,ti->B.pos,ti->C.pos,&Res)
			) {
				normal = (B.prev_pos-B.pos).normalize();
				Vector2f diff = (Res - B.pos);
				B.pos += diff;
				C.pos += diff;
				A.pos += diff;
				imp = calculate_impulse(B,Res,ti);
				apply_impulse(Res,normal,imp);
			}
			if (lines_intersect(pos,C.pos,ti->A.pos,ti->C.pos,&Res)
			 || lines_intersect(pos,C.pos,ti->A.pos,ti->B.pos,&Res)
			 || lines_intersect(pos,C.pos,ti->B.pos,ti->C.pos,&Res)
			) {
				normal = (C.prev_pos-C.pos).normalize();
				Vector2f diff = (Res - C.pos);
				B.pos += diff;
				C.pos += diff;
				A.pos += diff;
				imp = calculate_impulse(C,Res,ti);
				apply_impulse(Res,normal,imp);
			}
			*/
	

/*

			if (lines_intersect(ti->pos,ti->A.pos,A.pos,C.pos,&Res)
			 || lines_intersect(ti->pos,ti->A.pos,A.pos,B.pos,&Res)
			 || lines_intersect(ti->pos,ti->A.pos,B.pos,C.pos,&Res)
			) {
				apply_impulse(Res,(ti->A.pos-ti->A.prev_pos).normalize(),calculate_impulse(ti->A,Res));
			}
			if (lines_intersect(ti->pos,ti->B.pos,A.pos,C.pos,&Res)
			 || lines_intersect(ti->pos,ti->B.pos,A.pos,B.pos,&Res)
			 || lines_intersect(ti->pos,ti->B.pos,B.pos,C.pos,&Res)
			) {
				apply_impulse(Res,(ti->B.pos-ti->B.prev_pos).normalize(),calculate_impulse(ti->B,Res));
			}
			if (lines_intersect(ti->pos,ti->C.pos,A.pos,C.pos,&Res)
			 || lines_intersect(ti->pos,ti->C.pos,A.pos,B.pos,&Res)
			 || lines_intersect(ti->pos,ti->C.pos,B.pos,C.pos,&Res)
			) {
				apply_impulse(Res,(ti->C.pos-ti->C.prev_pos).normalize(),calculate_impulse(ti->C,Res));
			}
*/

		}
		void draw() {
			glColor3f(1.0,0.0,0.0);
			glLoadIdentity();
			glTranslatef(A.pos.x,A.pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				float z = i * (M_PI/180);
				glVertex2f(10*cos(z),10*sin(z));
			}
			glEnd();


			glColor3f(0.0,1.0,0.0);
			glLoadIdentity();
			glTranslatef(B.pos.x,B.pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				float z = i * (M_PI/180);
				glVertex2f(10*cos(z),10*sin(z));
			}
			glEnd();


			glColor3f(0.0,0.0,1.0);
			glLoadIdentity();
			glTranslatef(C.pos.x,C.pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				float z = i * (M_PI/180);
				glVertex2f(10*cos(z),10*sin(z));
			}
			glEnd();
			
			glColor3f(1.0,1.0,1.0);
			glLoadIdentity();
			glTranslatef(pos.x,pos.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				float z = i * (M_PI/180);
				glVertex2f(10*cos(z),10*sin(z));
			}
			glEnd();
			
			glColor3f(1.0,1.0,0.0);
			glLoadIdentity();
			glTranslatef(rot_center.x,rot_center.y,0);
			glBegin(GL_POLYGON);
			for (int i=0; i <= 360; i++) {
				float z = i * (M_PI/180);
				glVertex2f(10*cos(z),10*sin(z));
			}
			glEnd();

			
			glColor3f(1.0,1.0,1.0);
			glLoadIdentity();
			glBegin(GL_LINE_LOOP);
				glVertex2f(A.pos.x,A.pos.y);
				glVertex2f(B.pos.x,B.pos.y);
				glVertex2f(C.pos.x,C.pos.y);
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
				(*it)->update(dt);
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
			glLineWidth(5);
			glBegin(GL_LINES);
				glColor3f(1,0,0);
				glVertex2f(R1.x,R1.y);
				glVertex2f(R2.x,R2.y);
				
				glColor3f(0,0,1);
				glVertex2f(R3.x,R3.y);
				glVertex2f(R4.x,R4.y);
			
				glColor3f(0,1,0);
				glVertex2f(R5.x,R5.y);
				glVertex2f(R6.x,R6.y);
			glEnd();
			glLineWidth(1);
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


	world.add_triangle(Vector2f(300,200),100,100,100,false,-0*(M_PI/180));
	//world.add_triangle(Vector2f(300,50),100,100,100,false,-45*(M_PI/180));
	//world.add_triangle(Vector2f(300,350),100,100,100,false,-0*(M_PI/180));
	world.add_triangle(Vector2f(400,700),500,800,500,true,-180*(M_PI/180));

		
	//world.add_triangle(Vector2f(400,600),800,200,true,);
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
