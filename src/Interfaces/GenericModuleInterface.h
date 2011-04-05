#ifndef PEASANTFORMER_GENERIC_MODULE_INTERFACE
#define PEASANTFORMER_GENERIC_MODULE_INTERFACE

#include <stdio.h>
#include <string>
#include <vector>
#include <map>


class PeasantGenericModuleInterface {
	public:
		virtual ~PeasantGenericModuleInterface() {};
};

enum PeasantModuleType {
	PM_INVALID,
	PM_RENDER,
	PM_OBJECT,
	PM_TEST,
	PM_LAST
};


class PeasantGenericModuleInfo {
	protected:
		PeasantModuleType type;
		std::string name;
		std::string description;
		std::string version;
		std::string author;
	public:
		PeasantGenericModuleInfo(PeasantModuleType type
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
		virtual ~PeasantGenericModuleInfo() {};
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
		std::string get_author(void) {
			return this->author;
		}

};


typedef PeasantGenericModuleInterface *peasant_module_construct();
typedef void peasant_module_deconstruct(PeasantGenericModuleInterface *);
typedef PeasantGenericModuleInfo peasant_module_info();

class PeasantGenericModule {
	private:
		PeasantModuleType type;
		std::string filepath;
		std::string name;
		std::string description;
		std::string version;
		std::string author;
		peasant_module_construct *constructor;
		peasant_module_deconstruct *deconstructor;
	public:
		PeasantGenericModule() {
			this->type = PM_INVALID;
			this->filepath = "path/to/nowhere";
			this->name = "INVALID MODULE";
			this->description = "INVALID MODULE";
			this->version = "1.0";
			this->author = "Mr. Nobody";
			this->constructor = NULL;
			this->deconstructor = NULL;
		}
		PeasantGenericModule(PeasantModuleType type
		                    ,std::string filepath
		                    ,std::string name
		                    ,std::string description
		                    ,std::string version
		                    ,std::string author
		                    ,peasant_module_construct *constructor
		                    ,peasant_module_deconstruct *deconstructor
		                    ) {
			this->type = type;
			this->filepath = filepath;
			this->name = name;
			this->description = description;
			this->version = version;
			this->author = author;
			this->constructor = constructor;
			this->deconstructor = deconstructor;
		}
	public:
		PeasantModuleType get_type(void) {
			return this->type;
		}
		std::string get_filepath(void) {
			return this->filepath;
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
		peasant_module_construct *get_constructor(void) {
			return this->constructor;
		}
		peasant_module_deconstruct *get_deconstructor(void) {
			return this->deconstructor;
		}

};




#endif
