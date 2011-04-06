#ifndef PEASANTFORMER_CORE
#define PEASANTFORMER_CORE	  


#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

#include <peasantformer/Types.h>
#include <peasantformer/ObjectInterface.h>
#include <peasantformer/RenderInterface.h>




class PeasantCore {
	private:
		std::string modules_dir;
		std::vector<std::string> paths;
		std::map<std::string, PeasantGenericModule> modules;
		PeasantRenderModules render_modules;
		PeasantObjectModules object_modules;
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
		PeasantObjectModules *get_object_modules() {
			return &this->object_modules;
		}

};

#endif
