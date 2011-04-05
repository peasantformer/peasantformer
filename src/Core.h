#ifndef PEASANTFORMER_CORE
#define PEASANTFORMER_CORE	  

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


class PeasantCore {
	private:
		std::string modules_dir;
		std::vector<std::string> paths;
		std::map<std::string, PeasantGenericModule> modules;
		PeasantRenderModules render_modules;
		PeasantObjectModules object_modules;
//		std::vector<PeasantRenderModule> render_modules;
//		std::vector<PeasantObjectModule> object_modules;
	public:
		PeasantCore(std::string modules_dir);
	public:
		int scan_modules_dir(int recurse_depth = 0, std::string dirname = "modules");
		int load_modules();
		
		int load_module(std::string path);
		int unload_module();
	public:
		PeasantRenderModules *get_render_modules() {
			return &this->render_modules;
		}

};

#endif