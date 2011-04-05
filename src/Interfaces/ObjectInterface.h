#ifndef PEASANTFORMER_OBJECT_MODULE_INTERFACE
#define PEASANTFORMER_OBJECT_MODULE_INTERFACE

#include "GenericModuleInterface.h"

class PeasantObjectModuleInterface : public PeasantGenericModuleInterface {
	public:
		virtual ~PeasantObjectModuleInterface() {};
};



class ObjectInterface : public PeasantObjectModuleInterface {
	public:
		virtual ~ObjectInterface() {};
};

class JointInterface : public PeasantObjectModuleInterface {
	public:
		virtual ~JointInterface() {};
};

class RunnerInterface : public ObjectInterface {
	public:
		virtual ~RunnerInterface() {};
};

class CharacterInterface : public RunnerInterface {
	public:
		virtual ~CharacterInterface() {};
};

class PCInterface : public CharacterInterface {
	public:
		virtual ~PCInterface() {};
};

class NPCInterface : public CharacterInterface {
	public:
		virtual ~NPCInterface() {};
};

enum PeasantObjectModuleType {
	POM_INVALID = 0x0,
	POM_OBJECT  = 0x1,
	POM_JOINT   = 0x2,
	POM_RUNNER  = 0x4,
	POM_PC      = 0x8,
	POM_NPC     = 0x10,
	POM_LAST
};

class PeasantObjectModuleInfo {
	protected:
		PeasantObjectModuleType object_type;
	public:
		PeasantObjectModuleInfo(PeasantObjectModuleType object_type) {
			this->object_type = object_type;
		}
	public:
		PeasantObjectModuleType get_object_type(void) {
			return this->object_type;
		}
};

typedef PeasantObjectModuleInterface *peasant_object_module_construct();
typedef void peasant_object_module_deconstruct(PeasantObjectModuleInterface *);
typedef PeasantObjectModuleInfo peasant_object_module_info();

class PeasantObjectModule : public PeasantGenericModule {
	private:
		PeasantObjectModuleType object_type;
	public:
		PeasantObjectModule() {
			this->object_type = POM_INVALID;
		}
	public:
		PeasantObjectModule(PeasantObjectModuleType object_type
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
		PeasantObjectModuleType get_object_type(void) {
			return this->object_type;
		}

};

class PeasantObjectModules {
	private:
	std::map<std::string, PeasantObjectModule> objects;
		std::map<std::string, PeasantObjectModule> object_objects;
		std::map<std::string, PeasantObjectModule> joint_objects;
		
		std::map<std::string, PeasantObjectModule> runner_objects;
		std::map<std::string, PeasantObjectModule> pc_objects;
		std::map<std::string, PeasantObjectModule> npc_objects;
	public:
		PeasantObjectModules() {
		}
	public:
		int add_object(PeasantObjectModule m) {
			this->objects[m.get_name()] = m;
			if (m.get_object_type() & POM_OBJECT) {
				this->object_objects[m.get_name()] = m;
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
