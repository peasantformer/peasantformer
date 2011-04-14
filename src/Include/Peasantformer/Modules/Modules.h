#ifndef PEASANTFORMER_Modules_Modules
#define PEASANTFORMER_Modules_Modules

#include <stdio.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <dirent.h>
#include <errno.h>
#include <unistd.h>

#include <vector>
#include <string>
#include <map>

#include <Peasantformer/Modules/Interfaces/ModuleInterface.h>
#include <Peasantformer/Modules/Interfaces/RenderInterface.h>
#include <Peasantformer/Modules/Interfaces/ObjectInterface.h>

class Modules {
	private:
		bool renders;
		bool objects;
		std::vector<std::string> modules_dirs;
	public:
		Modules(bool renders=false, bool objects=false) {
			this->renders = renders;
			this->objects = objects;
		}
	public:
		std::map<std::string,RenderModule> render_modules;
		std::map<std::string,ObjectModule> object_modules;
	public:
		int add_dir(std::string dir); 
		int load_modules_in_dirs(std::string dir = "");
		int load_module(std::string path, bool renders=false, bool objects=false);
};

#endif
