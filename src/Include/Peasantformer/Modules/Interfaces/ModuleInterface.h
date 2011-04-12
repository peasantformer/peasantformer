#ifndef PEASANTFORMER_Modules_Interfaces_ModuleInterface
#define PEASANTFORMER_Modules_Interfaces_ModuleInterface

#include <string>

class ModuleInterface {
	public:
		virtual ~ModuleInterface() {};
};


enum ModuleType {
	MODULE_INVALID,
	MODULE_RENDER,
	MODULE_OBJECT,
	MODULE_LAST
};


class ModuleInfo {
	private:
		ModuleType type;
		std::string name;
		std::string description;
		std::string version;
		std::string author;
	public:
		ModuleInfo(ModuleType type
		          ,std::string name
		          ,std::string description
		          ,std::string version
		          ,std::string author) {
			this->type = type;
			this->name = name;
			this->description = description;
			this->version = version;
			this->author = author;
		}
		virtual ~ModuleInfo() {};
	public:
		ModuleType get_type(void) {
			return this->type;
		}
		std::string get_name(void) {
			return this->name;
		}
		std::string get_description(void) {
			return this->description;
		}
		std::string get_version(void) {
			return this->version;
		}
		std::string get_author(void) {
			return this->author;
		}
};

typedef ModuleInterface *module_construct();
typedef void module_destruct(ModuleInterface *);
typedef ModuleInfo module_info();


class Module {
	protected:
		ModuleType type;
		std::string name;
		std::string description;
		std::string version;
		std::string author;
		module_construct *constructor;
		module_destruct *destructor;
	public:
		Module() {
			this->type = MODULE_INVALID;
			this->name = "INVALID MODULE";
			this->description = "INVALID MODULE";
			this->version = "-1";
			this->author= "Mr. Nobody";
			this->constructor = NULL;
			this->destructor = NULL;
		}
		Module(ModuleType type
		      ,std::string name
		      ,std::string description
		      ,std::string version
		      ,std::string author) {
			this->type = type;
			this->name = name;
			this->description = description;
			this->version = version;
			this->author = author;
		}
		virtual ~Module() {};
	public:
		ModuleType get_type() {
			return this->type;
		}
		std::string get_name() {
			return this->name;
		}
		std::string get_description() {
			return this->description;
		}
		std::string get_version() {
			return this->version;
		}
		std::string get_author() {
			return this->author;
		}
};

#endif
