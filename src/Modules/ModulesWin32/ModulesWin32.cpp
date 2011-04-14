#include "ModulesWin32.h"

int Modules::load_module(std::string path, bool renders, bool objects) {
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
	ModuleInfo minfo = info();
	if (minfo.get_type() == MODULE_RENDER && renders) {
		render_module_info *render_info = (render_module_info*)GetProcAddress(module_ptr, "render_info");
		if (!render_info) {
			printf("Error loading render_info() from module %s, ignoring\n",path.c_str());
			return -1;
		}
		RenderInfo rinfo = render_info();
		this->render_modules[minfo.get_name()] = RenderModule(rinfo.get_render_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	}
	if (minfo.get_type() == MODULE_OBJECT && objects) {
		object_module_info *object_info = (object_module_info*)GetProcAddress(module_ptr, "object_info");
		if (!object_info) {
			printf("Error loading object_info() from module %s, ignoring\n",path.c_str());
			return -1;
		}
		ObjectInfo oinfo = object_info();
		this->object_modules[minfo.get_name()] = ObjectModule(oinfo.get_object_type(),minfo.get_name(),minfo.get_description(),minfo.get_version(),minfo.get_author());
	}
	return 0;
}
