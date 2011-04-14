#ifndef PEASANTFORMER_Modules_Interfaces_ObjectInterface
#define PEASANTFORMER_Modules_Interfaces_ObjectInterface

#include <Peasantformer/Modules/Interfaces/ModuleInterface.h>

#include <string>

class ObjectInterface : public ModuleInterface {
	public:
		virtual ~ObjectInterface() {};

};

class JointInterface : public ModuleInterface {
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
	OBJECT_INVALID = 0x00,
	OBJECT_OBJECT  = 0x01,
	OBJECT_JOINT   = 0x02,
	OBJECT_RUNNER  = 0x04,
	OBJECT_PC      = 0x08,
	OBJECT_NPX     = 0x10,
	OBJECT_LAST
	
};

class ObjectInfo {
	private:
		ObjectType object_type;
	public:
		ObjectInfo(ObjectType object_type) {
			this->object_type = object_type;
		}
	public:
		ObjectType get_object_type(void) {
			return this->object_type;
		}
};

typedef ObjectInterface *object_construct();
typedef void object_destruct(ObjectInterface *);
typedef ObjectInfo object_module_info();

class ObjectModule : public Module {
	private:
		ObjectType object_type;
		object_construct *constructor;
		object_destruct *destructor;
	public:
		ObjectModule() {
			this->object_type = OBJECT_INVALID;
			this->constructor = NULL;
			this->destructor = NULL;
		}
		ObjectModule(ObjectType object_type
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
