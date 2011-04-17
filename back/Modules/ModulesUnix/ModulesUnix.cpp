#include "ModulesUnix.h"


int Modules::load_module(std::string path, bool renders, bool objects) {
	const char *dlsym_error;
	void *module_ptr = dlopen(path.c_str(), RTLD_LAZY);
	
	

	if (module_ptr == NULL) {
		//printf("%s, ignoring\n",dlerror());
		return -1;
	}

	
	dlerror();
	
	module_construct *constructor = (module_construct*)dlsym(module_ptr, "construct");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("%s, ignoring\n", dlsym_error);
		dlclose(module_ptr);
		return -1;
	}
	
	module_destruct *destructor = (module_destruct*)dlsym(module_ptr, "destruct");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("%s, ignoring\n", dlsym_error);
		dlclose(module_ptr);
		return -1;
	}
	
	module_info *info = (module_info*)dlsym(module_ptr, "info");
	dlsym_error = dlerror();
	if (dlsym_error) {
		printf("%s, ignoring\n", dlsym_error);
		dlclose(module_ptr);
		return -1;
	}
	ModuleInfo minfo = info();
	if (minfo.get_type() == MODULE_RENDER && renders) {
		render_module_info *render_info = (render_module_info*)dlsym(module_ptr, "render_info");
		dlsym_error = dlerror();
		if (dlsym_error) {
			printf("%s, ignoring\n", dlsym_error);
			dlclose(module_ptr);
			return -1;
		}
		RenderInfo rinfo = render_info();
		this->render_modules[minfo.get_name()] = RenderModule(rinfo.get_render_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	}
	if (minfo.get_type() == MODULE_OBJECT && objects) {
		object_module_info *object_info = (object_module_info*)dlsym(module_ptr, "object_info");
		dlsym_error = dlerror();
		if (dlsym_error) {
			printf("%s, ignoring\n", dlsym_error);
			dlclose(module_ptr);
			return -1;
		}
		ObjectInfo oinfo = object_info();
		this->object_modules[minfo.get_name()] = ObjectModule(oinfo.get_object_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	}
	dlclose(module_ptr);
	return 0;
}
