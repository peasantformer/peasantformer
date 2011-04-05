#ifndef PEASANTFORMER_GENERIC_MODULE_INTERFACE
#define PEASANTFORMER_GENERIC_MODULE_INTERFACE

#include <string>

enum PeasantModuleType {
	PM_INVALID,
	PM_INCOMPLETE,
	PM_RENDER,
	PM_OBJECT,
	PM_TEST,
	PM_LAST
};

class PeasantModuleInfo {
	protected:
		PeasantModuleType type;
		std::string name;
		std::string description;
		std::string version;
	public:
		PeasantModuleInfo();
		PeasantModuleInfo(PeasantModuleType type
		                 ,std::string name
		                 ,std::string description
		                 ,std::string version) {
			this->type = type;
			this->name = name;
			this->description = description;
			this->version = version;
		}
		virtual ~PeasantModuleInfo() {};
	public:
		PeasantModuleType get_type(void) {
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

};

class PeasantGenericModuleInterface {
	public:
		virtual ~PeasantGenericModuleInterface() {};
	public:
		virtual void test() = 0;
};


typedef PeasantGenericModuleInterface *peasant_module_construct();
typedef void peasant_module_deconstruct(PeasantGenericModuleInterface *);
typedef PeasantModuleInfo peasant_module_info();

#endif
