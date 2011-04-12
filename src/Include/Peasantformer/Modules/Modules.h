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
#include <string>
#include <map>

#include <Peasantformer/Modules/Interfaces/ModuleInterface.h>
#include <Peasantformer/Modules/Interfaces/RenderInterface.h>
#include <Peasantformer/Modules/Interfaces/PeasantObjectInterface.h>

class Modules {
	private:
		std::vector<std::string> modules_dirs;
		std::map<std::string,Module> all_modules;
		std::map<std::string,RenderModule> render_modules;
		std::map<std::string,PeasantObjectModule> object_modules;
	public:
		int add_dir(std::string dir); 
		int load_modules_in_dirs(std::string dir = "");
		int load_module(std::string path);
};

int Modules::add_dir(std::string dir) {
	this->modules_dirs.push_back(dir);
	return 0;
}

int Modules::load_modules_in_dirs(std::string dir) {
	if (dir == "") {
		for(std::vector<std::string>::iterator it = this->modules_dirs.begin(); it != this->modules_dirs.end(); it++) {
			if (*it != "") {
				this->load_modules_in_dirs(*it);
			}
		}
		return 0;
	}	
	DIR *directory = NULL;
	struct dirent *derp = NULL;
	
	directory = opendir(dir.c_str());
	if (directory == NULL) {
		perror(dir.c_str());
		return -1;
	}
	std::string fname;
	while ((derp = readdir(directory)) != NULL) {
		if (derp->d_name == std::string(".")) continue;
		if (derp->d_name == std::string("..")) continue;
		fname = dir + "/" + derp->d_name;
		struct stat buf;
		stat(fname.c_str(),&buf);
		if (S_ISREG(buf.st_mode))	 {
			if (this->load_module(fname) == 0) {
				printf("%s module loaded\n",fname.c_str());
			}
		} else if (S_ISDIR(buf.st_mode)) {
			this->load_modules_in_dirs(fname);
		}
	}
	return 0;
}

int Modules::load_module(std::string path) {
#ifdef _WIN32
	HMODULE module_ptr = LoadLibrary(path.c_str());
	if (!module_ptr) {
		//printf("Error opening module %s, ignoring\n",path.c_str());
		return -1;
	}
	
	module_construct *constructor = (module_construct*)GetProcAddress(module_ptr, "construct");
	if (!constructor) {
		printf("Error loading constructor() from module %s, ignoring\n",path.c_str());
		return -1;
	}
	
	module_destruct *destructor = (module_destruct*)GetProcAddress(module_ptr, "destruct");
	if (!destructor) {
		printf("Error loading destructor() from module %s, ignoring\n",path.c_str());
		return -1;
	}
	
	module_info *info = (module_info*)GetProcAddress(module_ptr, "info");
	if (!info) {
		printf("Error loading info() from module %s, ignoring\n",path.c_str());
		return -1;
	}
#else	
	const char *dlsym_error;
	void *module_ptr = dlopen(path.c_str(), RTLD_LAZY);
	if (!module_ptr) {
		//printf("%s, ignoring\n",dlerror());
		return -1;
	}
	
	dlerror();
	
	module_construct *constructor = (module_construct*)dlsym(module_ptr, "construct");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("%s, ignoring\n", dlsym_error);
		return -1;
	}
	
	module_destruct *destructor = (module_destruct*)dlsym(module_ptr, "destruct");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("%s, ignoring\n", dlsym_error);
		return -1;
	}
	
	module_info *info = (module_info*)dlsym(module_ptr, "info");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("%s, ignoring\n", dlsym_error);
		return -1;
	}
#endif
	ModuleInfo minfo = info();
	this->all_modules[minfo.get_name()] = Module(minfo.get_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	if (minfo.get_type() == MODULE_RENDER) {
#ifdef _WIN32
		render_module_info *render_info = (render_module_info*)GetProcAddress(module_ptr, "render_info");
		if (!render_info) {
			printf("Error loading render_info() from module %s, ignoring\n",path.c_str());
			return -1;
		}
#else
		render_module_info *render_info = (render_module_info*)dlsym(module_ptr, "render_info");
		dlsym_error = dlerror();
		if (dlsym_error) {
			printf("%s, ignoring\n", dlsym_error);
			return -1;
		}
#endif
		RenderInfo rinfo = render_info();
		this->render_modules[minfo.get_name()] = RenderModule(rinfo.get_render_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	}
	if (minfo.get_type() == MODULE_OBJECT) {
#ifdef _WIN32
		object_module_info *object_info = (object_module_info*)GetProcAddress(module_ptr, "object_info");
		if (!object_info) {
			printf("Error loading object_info() from module %s, ignoring\n",path.c_str());
			return -1;
		}
#else
		object_module_info *object_info = (object_module_info*)dlsym(module_ptr, "object_info");
		dlsym_error = dlerror();
		if (dlsym_error) {
			printf("%s, ignoring\n", dlsym_error);
			return -1;
		}
#endif
		PeasantObjectInfo oinfo = object_info();
		this->object_modules[minfo.get_name()] = PeasantObjectModule(oinfo.get_object_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	}
	return 0;
}
