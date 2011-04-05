#ifndef PEASANTFORMER_MAIN
#define PEASANTFORMER_MAIN  

#include <string>
#include <vector>
#include <stdio.h>
#include <sys/types.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>
#include <dlfcn.h>
#include "Engine/Engine.h"
#include "Interfaces/GenericModuleInterface.h"
#include "Interfaces/RenderInterface.h"
#include "Interfaces/ObjectInterface.h"
#include "Modules/Test/Test.h"

class PeasantGenericModule {
	private:
		PeasantModuleType type;
		std::string filepath;
		std::string name;
		std::string description;
		std::string version;
		peasant_module_construct *constructor;
		peasant_module_deconstruct *deconstructor;
	public:
		PeasantGenericModule(PeasantModuleType type
		                    ,std::string filepath
		                    ,std::string name
		                    ,std::string description
		                    ,std::string version
		                    ,peasant_module_construct *constructor
		                    ,peasant_module_deconstruct *deconstructor
		                    ) {
			this->type = type;
			this->filepath = filepath;
			this->name = name;
			this->description = description;
			this->version = version;
			this->constructor = constructor;
			this->deconstructor = deconstructor;
		}

	public:
		PeasantModuleType get_type(void);
		std::string get_filepath(void);
		std::string get_name(void);
		std::string get_description(void);
		std::string get_version(void);
		peasant_module_construct * get_constructor(void);
		peasant_module_deconstruct * get_deconstructor(void);

};

class PeasantRenderModule : public PeasantGenericModule {
	private:
		
	public:
		PeasantRenderModuleInterface *instance;
	
};

class PeasantCore {
	private:
		std::string modules_dir;
		std::vector<std::string> paths;
		std::vector<PeasantGenericModule> modules;
	public:
		PeasantCore(std::string modules_dir);
	public:
		int scan_modules_dir(int recurse_depth = 0, std::string dirname = "modules");
		int load_modules();
		
		int load_module(std::string path);
		int unload_module();
		
		std::vector<PeasantGenericModule> get_modules() {
			return this->modules;
		}
};

#endif
