#include "Core.h"

PeasantCore::PeasantCore(std::string modules_dir) {
	this->modules_dir = modules_dir;
}
int PeasantCore::scan_modules_dir(int recurse_depth, std::string dirname) {
	DIR *directory = NULL;
	struct dirent *derp = NULL;
	if (recurse_depth == 0) {
		dirname = this->modules_dir;
	} 
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
			this->scan_modules_dir(recurse_depth + 1, fname);
		}
#else
		struct stat buf;
		stat(fname.c_str(),&buf);
		if (S_ISREG(buf.st_mode)) {	
			this->paths.push_back(fname);
		} else if (S_ISDIR(buf.st_mode)) {
			this->scan_modules_dir(recurse_depth + 1, fname);
		}
#endif
	}
	closedir(directory);
	return 0;
}
int PeasantCore::load_modules() {
	for (std::vector<std::string>::iterator it = this->paths.begin(); it!= this->paths.end(); it++) {
		if (this->load_module(*it) == 0) {
			printf("%s module loaded\n",it->c_str());
		}
	}
	return 0;
}

int PeasantCore::load_module(std::string path) {
#ifdef _WIN32
	HMODULE module_ptr = LoadLibrary(path.c_str());
	if (!module_ptr) {
		printf("Something has gone wrong on loading library %s\n",path.c_str()	);
		return -1;
	}
	
	peasant_module_construct* constructor = (peasant_module_construct*) GetProcAddress(module_ptr, "construct");
	if (!peasant_module_construct) {
		printf("Something has gone wrong on loading  'construct' from library %s\n",path.c_str());
		return -1;
	}

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
	
	//this->modules[minfo.get_name()] = PeasantGenericModule(minfo.get_type(), path, minfo.get_name(), minfo.get_description(), minfo.get_version(),minfo.get_author(),constructor,deconstructor);
	
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
int PeasantCore::unload_module() {
}


