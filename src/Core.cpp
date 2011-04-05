#include "Core.h"

PeasantModuleType PeasantGenericModule::get_type(void) {
	return this->type;
}
std::string PeasantGenericModule::get_filepath(void) {
	return this->filepath;
}
std::string PeasantGenericModule::get_name(void) {
	return this->name;
}
std::string PeasantGenericModule::get_description(void) {
	return this->description;
}
std::string PeasantGenericModule::get_version(void) {
	return this->version;
}
peasant_module_construct * PeasantGenericModule::get_constructor(void) {
	return this->constructor;
}
peasant_module_deconstruct * PeasantGenericModule::get_deconstructor(void) {
	return this->deconstructor;
}









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
	while ((derp = readdir(directory)) != NULL) {
		if (derp->d_name == std::string(".")) continue;
		if (derp->d_name == std::string("..")) continue;
		if (derp->d_type == DT_REG) {
			printf("Found module-like file %s\n",derp->d_name);
			this->paths.push_back(dirname + "/" + derp->d_name);
		} else if (derp->d_type == DT_DIR) {
			this->scan_modules_dir(recurse_depth + 1, dirname + "/" + derp->d_name);
		}
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
	
	PeasantModuleInfo minfo = info();
	
	this->modules.push_back(PeasantGenericModule(minfo.get_type(), path, minfo.get_name(), minfo.get_description(), minfo.get_version(),constructor,deconstructor))	;
	
	return 0;
}
int PeasantCore::unload_module() {
}






int main (int argc, char **argv) {
	PeasantCore *core;
	core = new PeasantCore("modules");
	
	core->scan_modules_dir();
	core->load_modules();
	//PeasantGenericTestModule *test = (PeasantGenericTestModule *)core->get_modules()[0].get_constructor()();
	PeasantGenericTestModule *test = (PeasantGenericTestModule *)core->get_modules()[0].get_constructor()();
	test->flip();
	//test->test();
	
	delete core;
	return 0;
}
