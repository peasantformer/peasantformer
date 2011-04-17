#ifndef PEASANTFORMER_Modules_Interfaces_RenderInterface
#define PEASANTFORMER_Modules_Interfaces_RenderInterface

#include <Peasantformer/Modules/Interfaces/ModuleInterface.h>

#include <string>

class RenderInterface : public ModuleInterface {
	public:
		virtual ~RenderInterface() {};
	public:
		virtual void render() = 0;
		virtual void clear() = 0;
		virtual void flip() = 0;
};

enum RenderType {
	RENDER_INVALID,
	RENDER_HARDWARE,
	RENDER_SOFTWARE,
	RENDER_LAST
};

class RenderInfo {
	private:
		RenderType render_type;
	public:
		RenderInfo(RenderType render_type) {
			this->render_type = render_type;
		}
	public:
		RenderType get_render_type(void) {
			return this->render_type;
		}
};

typedef RenderInterface *render_construct();
typedef void render_destruct(RenderInterface *);
typedef RenderInfo render_module_info();

class RenderModule : public Module {
	private:
		RenderType render_type;
		render_construct *constructor;
		render_destruct *destructor;
	public:
		RenderModule() {
			this->render_type = RENDER_INVALID;
			this->constructor = NULL;
			this->destructor = NULL;
		}
		RenderModule(RenderType render_type
		             ,std::string name
		             ,std::string description
		             ,std::string version
		             ,std::string author) : Module(MODULE_RENDER,name,description,version, author) {
			this->render_type = render_type;
		}
	public:
		RenderType get_render_type(void) {
			return this->render_type;
		}
};

#endif
