#include "Core.h"
PeasantCore::PeasantCore(char *modules_dir) {
	this->modules_dir = modules_dir;
}

PeasantCore::~PeasantCore() {

}

int PeasantCore::scan_modules(int recurse_depth, char *recurse_dirname) {
	DIR *directory = NULL;
	char *dirname = NULL;
	if (recurse_depth == 0) {
		dirname = this->modules_dir;
	} else {
		dirname = recurse_dirname;
	}
	directory = opendir(dirname);

	if (directory == NULL) {
		perror(dirname);
		return -1;
	}
	struct dirent *derp = NULL;
	while ((derp = readdir(directory)) != NULL) {
		if (strncmp(derp->d_name,".",2) == 0) continue;
		if (strncmp(derp->d_name,"..",3) == 0) continue;
		if (derp->d_type == DT_REG) {
			printf("Found module-like file %s\n",derp->d_name);
			this->modules_paths.push_back(std::string(dirname) + "/" + derp->d_name);
		}
		if (derp->d_type == DT_DIR) {
			char *path = (char*)malloc (((recurse_depth + 1) * 256 + 1) * sizeof (char));
			strncat(path,dirname,256);
			strncat(path,"/",1);
			strncat(path,derp->d_name,256);
			this->scan_modules(recurse_depth + 1,path);
			free(path);
		}
	}
	closedir(directory);
	return 0;
}

int PeasantCore::load_modules() {
	for(std::vector<std::string>::iterator it = this->modules_paths.begin(); it != this->modules_paths.end(); it++) {
		if (this->load_module((char*)it->data()) == 0) {
			printf("%s module loaded\n",it->data());
		} 
	}
	return 0;
}

int PeasantCore::load_module(char *modulepath) {
	void *module_ptr = dlopen(modulepath, RTLD_LAZY);
	if (!module_ptr) {
		printf("error: %s\n",dlerror());
		return -1;
	}
	
	dlerror();
	
	create_module_t* create_module = (create_module_t*) dlsym(module_ptr, "create");	
	const char* dlsym_error = dlerror();
	if (dlsym_error) {
		printf("error: %s\n",dlsym_error);
		return -1;
	}
	
	destroy_module_t* destroy_module = (destroy_module_t*) dlsym(module_ptr, "destroy");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("error: %s\n",dlsym_error);
		return -1;
	}
	
	ModuleInfo (*info_module)() = (ModuleInfo(*)())dlsym(module_ptr, "info");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("error: %s\n",dlsym_error);
		return -1;
	}
	ModuleInfo info = info_module();
	this->modules.push_back(PeasantModule(info.name,info.descr,info.version,create_module,destroy_module));
	this->modules[0].instance->test();
	
	return 0;
}

int PeasantCore::unload_module() {
}
