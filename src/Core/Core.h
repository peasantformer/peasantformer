#ifndef PEASANTFORMER_CORE
#define PEASANTFORMER_CORE

#include <sys/types.h>
#include <dirent.h>
#include <stdio.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <stdlib.h>
#include <string>
#include <vector>
#include <dlfcn.h>

#include "ModuleInterface.h"


class PeasantModule {
	private:
		char *name;
		char *descr;
		char *version;
		PeasantModuleInterface *(*constructor)();
		void (*destructor)(PeasantModuleInterface *);
	public:
		PeasantModuleInterface *instance;
	public:
		PeasantModule(char *name, char *descr, char *version, PeasantModuleInterface *(*constructor)(), void (*destructor)(PeasantModuleInterface *)) {
			this->name = name;
			this->descr = descr;
			this->version = version;
			this->constructor = constructor;
			this->destructor = destructor;
			this->instance = this->constructor();
		}
};

class PeasantCore {
	private:
		char *modules_dir;
		std::vector<std::string> modules_paths;
		std::vector<PeasantModule> modules;
	public:
		PeasantCore(char * modules_dir);
		~PeasantCore();
	public:
		int scan_modules(int recurse_depth = 0, char *recurse_dirname = NULL);
		int load_modules();
		
		int load_module(char *modulepath);
		int unload_module();
};

#endif
