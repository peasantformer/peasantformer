#ifndef PEASANTFORMER_Modules_Interfaces_PeasantObjectInterface
#define PEASANTFORMER_Modules_Interfaces_PeasantObjectInterface

#include <Peasantformer/Modules/Interfaces/ModuleInterface.h>

#include <string>

class PeasantObjectInterface : public ModuleInterface {
	public:
		virtual ~PeasantObjectInterface() {};

};

class ObjectInterface : public PeasantObjectInterface {
	public:
		virtual ~ObjectInterface() {};
};

class JointInterface : public PeasantObjectInterface {
	public:
		virtual ~JointInterface() {};
};

class RunnerInterface : public ObjectInterface {
	public:
		virtual ~RunnerInterface() {};
};

class PCInterface : public ObjectInterface {
	public:
		virtual ~PCInterface() {};
};

class NPCInterface : public ObjectInterface {
	public:
		virtual ~NPCInterface() {};
};


enum ObjectType {
	OBJECT_INVALID = 0x0,
	OBJECT_OBJECT  = 0x1,
	OBJECT_JOINT   = 0x2,
	OBJECT_RUNNER  = 0x4,
	OBJECT_PC      = 0x8,
	OBJECT_NPX     = 0x10,
	OBJECT_LAST
	
};

class PeasantObjectInfo {
	private:
		ObjectType object_type;
	public:
		PeasantObjectInfo(ObjectType object_type) {
			this->object_type = object_type;
		}
	public:
		ObjectType get_object_type(void) {
			return this->object_type;
		}
};

typedef PeasantObjectInterface *object_construct();
typedef void object_destruct(PeasantObjectInterface *);
typedef PeasantObjectInfo object_module_info();

class PeasantObjectModule : public Module {
	private:
		ObjectType object_type;
		object_construct *constructor;
		object_destruct *destructor;
	public:
		PeasantObjectModule() {
			this->object_type = OBJECT_INVALID;
			this->constructor = NULL;
			this->destructor = NULL;
		}
		PeasantObjectModule(ObjectType object_type
		             ,std::string name
		             ,std::string description
		             ,std::string version
		             ,std::string author) : Module(MODULE_RENDER,name,description,version, author) {
			this->object_type = object_type;
		}
	public:
		ObjectType get_object_type(void) {
			return this->object_type;
		}
};

#endif
