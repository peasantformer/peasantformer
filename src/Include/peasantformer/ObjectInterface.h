#ifndef PEASANTFORMER_OBJECT_MODULE_INTERFACE
#define PEASANTFORMER_OBJECT_MODULE_INTERFACE


#include <stdio.h>
#include "GenericModuleInterface.h"
#include "Types.h"

class PeasantObjectModuleInterface : public PeasantGenericModuleInterface {
	public:
		virtual ~PeasantObjectModuleInterface() {};
};

class ObjectInterface : public PeasantObjectModuleInterface {
	protected:
		Vector2f position;
		PeasantWorldSection *ul;
		
	public:
		virtual ~ObjectInterface() {};
};

class ParticleInterface : public ObjectInterface {
	public:
		virtual ~ParticleInterface() {};
};

class JointInterface : public ObjectInterface {
	public:
		virtual ~JointInterface() {};
};

class RunnerInterface : public ObjectInterface {
	public:
		virtual ~RunnerInterface() {};
};

class CharacterInterface : public ObjectInterface {
	public:
		virtual ~CharacterInterface() {};
};

class PCInterface : public ObjectInterface {
	public:
		virtual ~PCInterface() {};
};

class NPCInterface : public ObjectInterface {
	public:
		virtual ~NPCInterface() {};
};

enum PeasantObjectModuleType {
	POM_INVALID  = 0x0,
	POM_OBJECT   = 0x1,
	POM_PARTICLE = 0x2,
	POM_JOINT    = 0x4,
	POM_RUNNER   = 0x8,
	POM_PC       = 0x10,
	POM_NPC      = 0x20,
	POM_LAST
};

class PeasantObjectModuleInfo {
	protected:
		int object_type;
	public:
		PeasantObjectModuleInfo(int object_type) {
			this->object_type = object_type;
		}
	public:
		int get_object_type(void) {
			return this->object_type;
		}
};

typedef PeasantObjectModuleInterface *peasant_object_module_construct();
typedef void peasant_object_module_deconstruct(PeasantObjectModuleInterface *);
typedef PeasantObjectModuleInfo peasant_object_module_info();

class PeasantObjectModule : public PeasantGenericModule {
	private:
		int object_type;
	public:
		PeasantObjectModule() {
			this->object_type = POM_INVALID;
		}
	public:
		PeasantObjectModule(int object_type
		                   ,std::string filepath
		                   ,std::string name
		                   ,std::string description
		                   ,std::string version
		                   ,std::string author
		                   ,peasant_module_construct *constructor
		                   ,peasant_module_deconstruct *deconstructor
		                   ) : PeasantGenericModule(PM_OBJECT,filepath,name,description,version,author,constructor,deconstructor) {
			this->object_type = object_type;
		}
	
	public:
		int get_object_type(void) {
			return this->object_type;
		}

};

class PeasantObjectModules {
	private:
	std::map<std::string, PeasantObjectModule> objects;
		std::map<std::string, PeasantObjectModule> object_objects;

		std::map<std::string, PeasantObjectModule> particle_objects;
		std::map<std::string, PeasantObjectModule> joint_objects;

		std::map<std::string, PeasantObjectModule> runner_objects;
		std::map<std::string, PeasantObjectModule> pc_objects;
		std::map<std::string, PeasantObjectModule> npc_objects;
	public:
		PeasantObjectModules() {
		}
	public:
		void print() {
			printf("Following Object modules were found:\n");
			printf("  Regular objects:\n");
			if (this->object_objects.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantObjectModule>::iterator it = this->object_objects.begin(); it != this->object_objects.end(); it++) {
				PeasantObjectModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			printf("  Particle objects:\n");
			if (this->particle_objects.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantObjectModule>::iterator it = this->particle_objects.begin(); it != this->particle_objects.end(); it++) {
				PeasantObjectModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			printf("  Joint objects:\n");
			if (this->joint_objects.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantObjectModule>::iterator it = this->joint_objects.begin(); it != this->joint_objects.end(); it++) {
				PeasantObjectModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			printf("  Runner objects:\n");
			if (this->runner_objects.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantObjectModule>::iterator it = this->runner_objects.begin(); it != this->runner_objects.end(); it++) {
				PeasantObjectModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			printf("  PC objects:\n");
			if (this->pc_objects.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantObjectModule>::iterator it = this->pc_objects.begin(); it != this->pc_objects.end(); it++) {
				PeasantObjectModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			printf("  NPC objects:\n");
			if (this->npc_objects.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantObjectModule>::iterator it = this->npc_objects.begin(); it != this->npc_objects.end(); it++) {
				PeasantObjectModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			
		}
		int add_object(PeasantObjectModule m) {
			this->objects[m.get_name()] = m;
			if (m.get_object_type() & POM_OBJECT) {
				this->object_objects[m.get_name()] = m;
			}
			if (m.get_object_type() & POM_PARTICLE) {
				this->particle_objects[m.get_name()] = m;
			}
			if (m.get_object_type() & POM_JOINT) {
				this->joint_objects[m.get_name()] = m;
			}
			if (m.get_object_type() & POM_RUNNER) {
				this->runner_objects[m.get_name()] = m;
			}
			if (m.get_object_type() & POM_PC) {
				this->pc_objects[m.get_name()] = m;
			}
			if (m.get_object_type() & POM_NPC) {
				this->npc_objects[m.get_name()] = m;
			}
		}
};
#endif
