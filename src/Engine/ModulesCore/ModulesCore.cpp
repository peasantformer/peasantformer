#include "ModulesCore.h"

PeasantModulesCore::PeasantModulesCore() {
	this->modules_dirs.push_back("modules");
	this->modules_dirs.push_back("~/.config/peasantformer/modules");
}
int PeasantModulesCore::scan_modules_dirs() {
	for (std::vector<std::string>::iterator it = this->modules_dirs.begin(); it != this->modules_dirs.end(); it++) {
		this->scan_modules_dir(*it);
	}
}
int PeasantModulesCore::scan_modules_dir(std::string dirname, int recurse_depth) {
	DIR *directory = NULL;
	struct dirent *derp = NULL;
	//if (recurse_depth == 0) {
	//	dirname = this->modules_dir;
	//} 
	directory = opendir(dirname.c_str());
	if (directory == NULL) {
		perror(dirname.c_str());
		return -1;
	}
	std::string fname;
	while ((derp = readdir(directory)) != NULL) {
		if (derp->d_name == std::string(".")) continue;
		if (derp->d_name == std::string("..")) continue;
		fname = dirname + "/" + derp->d_name;
#ifdef _DIRENT_HAVE_D_TYPE
		if (derp->d_type == DT_REG) {
			this->paths.push_back(fname);
		} else if (derp->d_type == DT_DIR) {
			this->scan_modules_dir(fname, recurse_depth + 1);
		}
#else
		struct stat buf;
		stat(fname.c_str(),&buf);
		if (S_ISREG(buf.st_mode)) {	
			this->paths.push_back(fname);
		} else if (S_ISDIR(buf.st_mode)) {
			this->scan_modules_dir(fname, recurse_depth + 1);
		}
#endif
	}
	closedir(directory);
	return 0;
}
int PeasantModulesCore::load_modules() {
	for (std::vector<std::string>::iterator it = this->paths.begin(); it!= this->paths.end(); it++) {
		if (this->load_module(*it) == 0) {
			printf("%s module loaded\n",it->c_str());
		}
	}
	return 0;
}

int PeasantModulesCore::load_module(std::string path) {
#ifdef _WIN32
	HMODULE module_ptr = LoadLibrary(path.c_str());
	if (!module_ptr) {
		printf("Something has gone wrong on loading module %s\n",path.c_str()	);
		return -1;
	}
	
	peasant_module_construct* constructor = (peasant_module_construct*) GetProcAddress(module_ptr, "construct");
	if (!constructor) {
		printf("Something has gone wrong on loading 'construct' from module %s\n",path.c_str());
		return -1;
	}
	
	peasant_module_deconstruct* deconstructor = (peasant_module_deconstruct*) GetProcAddress(module_ptr, "deconstruct");
	if (!deconstructor) {
		printf("Something has gone wrong on loading 'deconstruct' from module %s\n",path.c_str());
		return -1;
	}
	
	peasant_module_info* info = (peasant_module_info *) GetProcAddress(module_ptr, "info");
	if (!info) {
		printf("Something has gone wrong on loading 'info' from module %s\n",path.c_str());
		return -1;
	}

	PeasantGenericModuleInfo minfo = info();

	if (minfo.get_type() == PM_RENDER) {
		
		peasant_render_module_info* render_info = (peasant_render_module_info *) GetProcAddress(module_ptr, "render_info");
		if (!render_info) {
			printf("Something has gone wrong on loading 'render_info' from module %s\n",path.c_str());
			return -1;
		}
		
		PeasantRenderModuleInfo rminfo = render_info();
		this->render_modules.add_render(PeasantRenderModule(rminfo.get_render_type(), path, minfo.get_name(), minfo.get_description(), minfo.get_version(),minfo.get_author(),constructor,deconstructor));
	}
	
	if (minfo.get_type() == PM_OBJECT) {

		peasant_object_module_info* object_info = (peasant_object_module_info *) GetProcAddress(module_ptr, "object_info");
		if (!object_info) {
			printf("Something has gone wrong on loading 'object_info' from module %s\n",path.c_str());
			return -1;
		}
		
		PeasantObjectModuleInfo ominfo = object_info();
		this->object_modules.add_object(PeasantObjectModule(ominfo.get_object_type(), path, minfo.get_name(), minfo.get_description(), minfo.get_version(),minfo.get_author(),constructor,deconstructor));
	}
	

	return 0;
#else
	const char* dlsym_error;
	void *module_ptr = dlopen(path.c_str(), RTLD_LAZY);
	if (!module_ptr) {
		printf("error: %s\n",dlerror());
		return -1;
	}

	dlerror();
	
	peasant_module_construct* constructor = (peasant_module_construct*) dlsym(module_ptr, "construct");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("error: %s\n",dlsym_error);
		return -1;
	}
	
	peasant_module_deconstruct* deconstructor = (peasant_module_deconstruct*) dlsym(module_ptr, "deconstruct");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("error: %s\n",dlsym_error);
		return -1;
	}
	peasant_module_info *info = (peasant_module_info*) dlsym(module_ptr, "info");
	dlsym_error = dlerror();
	if (dlsym_error) {	
		printf("error: %s\n",dlsym_error);
		return -1;
	}
	
	PeasantGenericModuleInfo minfo = info();

	if (minfo.get_type() == PM_RENDER) {
		peasant_render_module_info *render_info = (peasant_render_module_info*) dlsym(module_ptr, "render_info");
		dlsym_error = dlerror();
		if (dlsym_error) {	
			printf("error: %s\n",dlsym_error);
			return -1;
		}
		PeasantRenderModuleInfo rminfo = render_info();
		this->render_modules.add_render(PeasantRenderModule(rminfo.get_render_type(), path, minfo.get_name(), minfo.get_description(), minfo.get_version(),minfo.get_author(),constructor,deconstructor));
	}
	if (minfo.get_type() == PM_OBJECT) {
		peasant_object_module_info *object_info = (peasant_object_module_info*) dlsym(module_ptr, "object_info");
		dlsym_error = dlerror();
		if (dlsym_error) {	
			printf("error: %s\n",dlsym_error);
			return -1;
		}
		PeasantObjectModuleInfo ominfo = object_info();
		this->object_modules.add_object(PeasantObjectModule(ominfo.get_object_type(), path, minfo.get_name(), minfo.get_description(), minfo.get_version(),minfo.get_author(),constructor,deconstructor));
	}
	
	return 0;
#endif
}
int PeasantModulesCore::unload_module() {
}


