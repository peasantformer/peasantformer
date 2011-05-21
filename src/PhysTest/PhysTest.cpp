#include "PhysTest.h"

class Timer {
	private:
			Uint32 startTicks;
			Uint32	pausedTicks;
			bool paused;
			bool started;
	public:
		Timer() {
			this->startTicks = 0;
			this->pausedTicks = 0;
			this->paused = false;
			this->started = false;
		}
		void start() {
			this->startTicks = SDL_GetTicks();
			this->pausedTicks = 0;
			this->started =  true;
			this->paused = false;
		}

		void stop() {
			this->startTicks = SDL_GetTicks() - this->startTicks;
			this->pausedTicks = 0;
			this->started = false;
			this->paused = false;
		}

		void pause() {
			if (this->started == true && this->paused == false) {
				this->pausedTicks = SDL_GetTicks();
				this->paused = true;
			}
		}
		void unpause() {
			if (this->started == true && this->paused == true) {
				this->startTicks += SDL_GetTicks() - this->pausedTicks;
				this->pausedTicks = 0;
				this->paused = false;
			}
		}

		Uint32 get_ticks() {
			if (this->started == true) {
				if (this->paused == true) {
					return (SDL_GetTicks() - (this->startTicks + (SDL_GetTicks() - this->pausedTicks)));
				} else {
					return SDL_GetTicks() - this->startTicks;
				}
			} else {
				return this->startTicks;
			}
		}
	public:
		bool get_paused(void) {
			return this->paused;
		}
		bool get_started(void) {
			return this->started;
		}

};

class Color3f {
	public:
		float r;
		float g;
		float b;
	public:
		Color3f() {
			this->r = 0.5;
			this->g = 0.5;
			this->b = 0.5;
		}
		Color3f(float r, float g, float b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}
	public:
		void Set(float r, float g, float b) {
			this->r = r;
			this->g = g;
			this->b = b;
		}
	
};

class PeasantObject;
class PeasantObjectGround;
class PeasantObjectRectangle;
class PeasantObjectBox;

class Visitor {
	protected:
		Visitor() {
		}
	public:
		virtual ~Visitor() {};
	public:
		virtual void visit(PeasantObject &) = 0;
		virtual void visit(PeasantObjectGround &) = 0;
		virtual void visit(PeasantObjectRectangle &) = 0;
		virtual void visit(PeasantObjectBox &) = 0;
};

class PeasantObject {
	public:
		b2Vec2 position;
		float angle;
		Color3f color;
		b2Body *body;
	public:
		PeasantObject(b2Vec2 position, float angle) {
			this->position = position;
			this->angle = angle;
			this->body = NULL;
		}
		virtual ~PeasantObject() {}
	public:
		virtual void accept(Visitor &v) {
			v.visit(*this);
		}
};

class PeasantObjectRectangle : public PeasantObject {
	public:
		b2Vec2 dimensions;
	public:
		PeasantObjectRectangle(b2Vec2 position, float angle, b2Vec2 dimensions) :
			PeasantObject(position,angle)
		{
			this->dimensions = dimensions;
		}
		virtual ~PeasantObjectRectangle() {}
	public:
		virtual void accept(Visitor &v) {
			v.visit(*this);
		}
};

class PeasantObjectGround : public PeasantObjectRectangle {
	public:
		PeasantObjectGround(b2Vec2 position, float angle, b2Vec2 dimensions) :
			PeasantObjectRectangle(position,angle,dimensions)
		{
		}
		virtual ~PeasantObjectGround() {}
	public:
		virtual void accept(Visitor &v) {
			v.visit(*this);
		}
};

class PeasantObjectBox : public PeasantObjectRectangle {
	public:
		PeasantObjectBox(b2Vec2 position, float angle, b2Vec2 dimensions) :
			PeasantObjectRectangle(position,angle,dimensions)
		{
		}
		virtual ~PeasantObjectBox() {}
	public:
		virtual void accept(Visitor &v) {
			v.visit(*this);
		}
};

class World {
	public:
		b2AABB worldAABB;
		b2Vec2 gravity;
		bool doSleep;
		float timeStep;
		int32 iterations;
		b2World *world;
	public:
		World() {

			this->worldAABB.lowerBound.Set(-1000,-1000);
			this->worldAABB.upperBound.Set(1000,1000);
			this->gravity = b2Vec2(0,-9.8);
			this->doSleep = true;
			this->timeStep = 1.0 / 60.0;
			this->iterations = 10;

			this->world = new b2World(worldAABB,gravity,doSleep);
		}
		~World() {
			delete this->world;
		}
	public:
		void iterate() {
			world->Step(timeStep,iterations);
		}
	public:
		PeasantObjectGround *addPlane(b2Vec2 position, float angle, b2Vec2 dimensions) {
			b2BodyDef plane_def;
			plane_def.position = position;
			plane_def.angle = angle;
			
			b2Body* plane = world->CreateBody(&plane_def);
			
			b2PolygonDef shape;
			shape.SetAsBox(dimensions.x, dimensions.y);
			
			plane->CreateShape(&shape);
			
			PeasantObjectGround *obj = new PeasantObjectGround(position, angle, dimensions);
			obj->body = plane;
			plane->SetUserData(obj);

			return obj;
		}
		PeasantObjectBox *addBox(
				b2Vec2 position, 
				float angle, 
				b2Vec2 dimensions,
				float restitution = 0.5,
				float friction = 0.3,
				float density = 1
			) {
			b2BodyDef box_def;
			box_def.position = position;
			box_def.angle = angle;

			b2Body *box = world->CreateBody(&box_def);

			b2PolygonDef shape;
			shape.SetAsBox(dimensions.x,dimensions.y);
			
			shape.restitution = restitution;
			shape.density = density;
			shape.friction = friction;

			box->CreateShape(&shape);
	
			box->SetMassFromShapes();


			PeasantObjectBox *obj = new PeasantObjectBox(position,angle,dimensions);
			obj->body = box;

			box->SetUserData(obj);

			return obj;
		}
};

class Render : public Visitor {
	public:
		virtual ~Render() {}
	public:
		virtual void init(
				int screen_width, 
				int screen_height, 
				int bpp, 
				string title
		) = 0;
		virtual void clear() = 0;
		virtual void flip() = 0;


		virtual void visit(PeasantObject &) = 0;
		virtual void visit(PeasantObjectRectangle &) = 0;
		virtual void visit(PeasantObjectGround &) = 0;
		virtual void visit(PeasantObjectBox &) = 0;
};

enum RenderType {
	RT_SDL_OPENGL
};

class RenderSDLOpengl : public Render {
	public:
		int screen_width;
		int screen_height;
		int bpp;
		string title;

		float scaling_factor;
	public:
		RenderSDLOpengl() {
		}
		~RenderSDLOpengl() {}
	public:
		void init(int screen_width, int screen_height, int bpp, string title) {
			this->screen_width = screen_width;
			this->screen_height = screen_height;
			this->bpp = bpp;
			this->title = title;

			this->scaling_factor = screen_width / 100.0;

			SDL_Init(SDL_INIT_EVERYTHING);
			SDL_SetVideoMode(screen_width,screen_height,bpp, SDL_OPENGL);
			SDL_WM_SetCaption(title.c_str(),NULL);
			glClearColor(0,0,0,0);
			glMatrixMode(GL_PROJECTION);
			glLoadIdentity();
			glOrtho(0,screen_width,screen_height,0,-1,1);
			glMatrixMode(GL_MODELVIEW);
			glLoadIdentity();
		}
		void clear() {
			glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		}
		void flip() {
			SDL_GL_SwapBuffers();
		}
	private:
		void renderBox(b2Vec2 position, float angle, b2Vec2 dimensions, Color3f color) {
			glColor3f(color.r,color.g,color.b);
			glLoadIdentity();
			glTranslatef(
					position.x*scaling_factor,
					position.y*scaling_factor*-1 + screen_height,
					0
			);
			glRotatef(angle*-1*(180/M_PI),0,0,1);
			glBegin(GL_POLYGON);
				glVertex2f(-dimensions.x*scaling_factor,-dimensions.y*scaling_factor);
				glVertex2f(dimensions.x*scaling_factor,-dimensions.y*scaling_factor);
				glVertex2f(dimensions.x*scaling_factor,dimensions.y*scaling_factor);
				glVertex2f(-dimensions.x*scaling_factor,dimensions.y*scaling_factor);
			glEnd();
		}
	public:
		void visit(PeasantObject &obj) {
			printf("Rendering generic object\n");
		}
		void visit(PeasantObjectRectangle &obj) {
			renderBox(obj.body->GetPosition(),obj.body->GetAngle(),obj.dimensions,obj.color);
		}
		void visit(PeasantObjectGround &obj) {
			renderBox(obj.body->GetPosition(),obj.body->GetAngle(),obj.dimensions,obj.color);
		}
		void visit(PeasantObjectBox &obj) {
			renderBox(obj.body->GetPosition(),obj.body->GetAngle(),obj.dimensions,obj.color);
		}

};

class Engine {
	public:
		Render *render;
		World *world;
		Visitor *visitor;
	public:
		Engine(RenderType rtype) {
			switch (rtype) {
				case RT_SDL_OPENGL:
					this->render = new RenderSDLOpengl;
					break;
			}
			this->world = new World;
		}
		~Engine() {
			delete this->world;
			delete this->render;
		}
	public:
		void init(
				int screen_width = 800, 
				int screen_height = 600, 
				int bpp = 32, 
				string title = "Peasantformer"
			) {
			render->init(screen_width,screen_height,bpp,title);
		}
		void mainLoop() {
			SDL_Event event;
			bool quit = false;

			Timer intervaller;
			Timer frame_meter;
			intervaller.start();
			frame_meter.start();

			float max_fps = 100;
			float min_fps= 1;
			float update_interval = 1.0 / max_fps;
			float max_cycles_per_frame = max_fps / min_fps;

			float last_frame_time = 0;
			float cycles_left_over = 0;
			float current_time;
			float update_iterations;

			int frames = 0;


			while (!quit) {
				
				current_time=(float)SDL_GetTicks()/1000.0;
				update_iterations = ((current_time - last_frame_time) + cycles_left_over);
				if (update_iterations > (max_cycles_per_frame * update_interval)) {
					update_iterations = max_cycles_per_frame * update_interval;
				}
				
				while (update_iterations >= update_interval) {
					SDL_PollEvent(&event);
					if (event.type == SDL_QUIT)
						quit = true;
					update_iterations -= update_interval;
					world->iterate();
					render->clear();			

					for (
						b2Body *p = world->world->GetBodyList(); 
						p->GetNext() != NULL; 
						p = p->GetNext()
					) {
						PeasantObject *pobject = (PeasantObject *)p->GetUserData();
						pobject->accept(*render);
					}

					render->flip();
				

				}
				frames++;

				if (frame_meter.get_ticks() > 1000 ) {
					printf("TICK %f @ %f\n",frames / (double)(frame_meter.get_ticks()/1000),(double)(frame_meter.get_ticks())/frames);
					frame_meter.start();
					frames = 0;
				}

				cycles_left_over = update_iterations;
				last_frame_time = current_time;
			}
		}
		void make_scene(void) {
			PeasantObjectGround *ground = this->world->addPlane(b2Vec2(50,0),0.0,b2Vec2(50,10));
			ground->color.Set(0.5,0.2,0.2);
			for (int n=0; n < 20; n++) {
			for (int i=0; i < 100; i++) {
			PeasantObjectBox *box = this->world->addBox(
					b2Vec2(10+i*0.7,70-n*0.7),
					-20*(M_PI/180),
					b2Vec2(0.2,0.2),
					0.1,
					0.5
			);
			}
			}
//			ground->color.Set(0.5,0.2,0.2);
		}

};


int main(int argc, char **argv) {
	Engine *engine = new Engine(RT_SDL_OPENGL);

	engine->init(800,600,32,",д,Я,Й,Б,");

	engine->make_scene();

	engine->mainLoop();
}

/*
enum BodyType {
	BT_GENERIC,
	BT_GROUND,
	BT_DYN_RECTANGLE,
	BT_LAST
};

float pusf = 10;
int screen_width = 800;
int screen_height = 600;

class BodyInfo {
	public:
		std::string name;
		BodyType type;
		b2Vec2 dimensions;
	public:
		BodyInfo() {
			this->name = "Generic object";
			this->type = BT_GENERIC;
		}
		BodyInfo(b2Vec2 dimensions) {
			this->dimensions = dimensions;
		}
		virtual ~BodyInfo() {};
	public:
		virtual void render() = 0;
};

class BodyInfo_ground : public BodyInfo {
	public:
		b2Vec2 dimensions;
	public:
		BodyInfo_ground(b2Vec2 dimensions) {
			this->name = "Ground object";
			this->type = BT_GROUND;
			this->dimensions = dimensions;
		}
	public:
		void render() {}
		void render(b2Vec2 pos, float32 angle) {

		}
};

class BodyInfo_rectangle : public BodyInfo {
	public:
		b2Vec2 dimensions;
	public:
		BodyInfo_rectangle(b2Vec2 dimensions) {
			this->name = "Rectangle object";
			this->type = BT_DYN_RECTANGLE;
			this->dimensions = dimensions;
		}
	public:
		void render() {};
		void render(b2Vec2 pos, float32 angle) {
			printf("%f\n",dimensions.y);

			glColor3f(1.0,1.0,0.0);
			glLoadIdentity();
			glTranslatef(pos.x*pusf,pos.y*pusf*-1+screen_width,0);
			glBegin(GL_POLYGON);
				glVertex2f(-dimensions.x*pusf,-dimensions.y*pusf);
				glVertex2f(dimensions.x*pusf,-dimensions.y*pusf);
				glVertex2f(dimensions.x*pusf,dimensions.y*pusf);
				glVertex2f(-dimensions.x*pusf,dimensions.y*pusf);
			glEnd();
		}
};


class World {
	public:
		b2AABB worldAABB;
		b2Vec2 gravity;
		bool doSleep;
		float32 timeStep;
		int32 iterations;
		b2World *world;

	public:
		World() {
			this->worldAABB.lowerBound.Set(-100,-100);
			this->worldAABB.upperBound.Set(100,100);
			this->gravity = b2Vec2(0,-0.1);
			this->doSleep = true;
			this->timeStep = 1.0 / 60.0;
			this->iterations = 10;

			this->world = new b2World(worldAABB,gravity,doSleep);
		}
	public:
		void iterate() {
			world->Step(timeStep,iterations);
		}
	public:
		void draw() {
			for (b2Body *p = world->GetBodyList(); p->GetNext() != NULL; p = p->GetNext()) {
				b2Vec2 pos = p->GetPosition();
				float32 angle = p->GetAngle();
				BodyInfo *info = (BodyInfo *)p->GetUserData();

				switch (info->type) {
					case BT_GENERIC:
						info->render();
						break;
					case BT_GROUND:
						((BodyInfo_ground*)p->GetUserData())->render(pos,angle);
						break;
					case BT_DYN_RECTANGLE:
						((BodyInfo_rectangle*)p->GetUserData())->render(pos,angle);
						break;
					case BT_LAST:
						break;
				}
				//printf("%d\n",info->type);
			}
		}
	public:
		void addBody_ground(b2Vec2 pos, b2Vec2 dimensions) {
			b2BodyDef groundBodyDef;
			groundBodyDef.position = pos;
			
			b2Body* ground = world->CreateBody(&groundBodyDef);
			
			b2PolygonDef groundShapeDef;
			groundShapeDef.SetAsBox(dimensions.x, dimensions.y);
			
			ground->CreateShape(&groundShapeDef);

			ground->SetUserData(new BodyInfo_ground(dimensions));
		}
		void addDynBody_rectangle(b2Vec2 pos, b2Vec2 dimensions) {
			b2BodyDef bodyDef;
			bodyDef.position = pos;

			b2Body *body = world->CreateBody(&bodyDef);

			b2PolygonDef shapeDef;
			shapeDef.SetAsBox(dimensions.x,dimensions.y);
			shapeDef.density = 1;
			shapeDef.friction = 0.3;

			body->CreateShape(&shapeDef);
			body->SetMassFromShapes();
			
			body->SetUserData(new BodyInfo_rectangle(dimensions));
		}
};


int main (int argc, char **argv) {
	SDL_Init(SDL_INIT_EVERYTHING);
	SDL_SetVideoMode(screen_width,screen_height,32, SDL_OPENGL);
	SDL_WM_SetCaption(",д,Я,Й,Б,",NULL);
	glClearColor(0,0,0,0);
	glMatrixMode(GL_PROJECTION);
	glLoadIdentity();
	glOrtho(0,screen_width,screen_height,0,-1,1);
	glMatrixMode(GL_MODELVIEW);
	glLoadIdentity();
	
	World world;


	world.addBody_ground(b2Vec2(0,10),b2Vec2(50,10));
	world.addDynBody_rectangle(b2Vec2(10,50),b2Vec2(1,1));

	b2Vec2 gravity(0.0f, -0.9f);
	bool doSleep = true;
	b2AABB worldAABB;
	worldAABB.lowerBound.Set(-100.0f, -100.0f);
	worldAABB.upperBound.Set(100.0f, 100.0f);
	
	b2World world(worldAABB, gravity, doSleep);

	b2BodyDef groundBodyDef;
	groundBodyDef.position.Set(0.0f, -10.0f);
	b2Body* ground = world.CreateBody(&groundBodyDef);
	b2PolygonDef groundShapeDef;
	groundShapeDef.SetAsBox(50.0f, 10.0f);
	ground->CreateShape(&groundShapeDef);





	b2BodyDef bodyDef;
	bodyDef.position.Set(0.0f, 50.0f);
	bodyDef.angle = 0.30 * b2_pi;
	b2Body* body = world.CreateBody(&bodyDef);

	b2PolygonDef shapeDef;
	shapeDef.SetAsBox(1.0f, 1.0f);
	shapeDef.density = 1.0f;
	shapeDef.friction = 0.3f;
	shapeDef.restitution = 1;
	body->CreateShape(&shapeDef);
	body->SetMassFromShapes();

	float32 timeStep = 1.0f / 60.0f;
	int32 iterations = 10;

	SDL_Event event;
	bool quit = false;


	while (quit == false) {
		SDL_PollEvent(&event);
		if (event.type == SDL_QUIT)
			quit = true;
	
		world.iterate();
//		world.Step(timeStep,iterations);
//		b2Vec2 position = body->GetPosition();
//		float32 angle = body->GetAngle();
//		printf("%4.2f %4.2f %4.2f\n", position.x, position.y, angle);

		glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

		world.draw();

		SDL_GL_SwapBuffers();

	}
a}
*/
