#ifndef PEASANTFORMER_ENGINE_MODULES_CORE
#define PEASANTFORMER_ENGINE_MODULES_CORE


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


#include <vector>

#include <peasantformer/Types.h>
#include <peasantformer/ObjectInterface.h>
#include <peasantformer/RenderInterface.h>




class PeasantModulesCore {
	friend class PeasntEngine;
	private:
		std::vector<std::string> modules_dirs;
		std::vector<std::string> paths;
		PeasantRenderModules render_modules;
		PeasantObjectModules object_modules;
	public:
		PeasantModulesCore();
	public:
		int scan_modules_dirs();
		int scan_modules_dir(std::string dirname, int recurse_depth = 0);
		int load_modules();
		
		int load_module(std::string path);
		int unload_module();
};

#endif
