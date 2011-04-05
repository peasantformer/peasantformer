#ifndef PEASANTFORMER_RENDER_MODULE_INTERFACE
#define PEASANTFORMER_RENDER_MODULE_INTERFACE

#include "GenericModuleInterface.h"

enum PeasantRenderModuleType {
	PRM_INVALID,
	PRM_INCOMPLETE,
	PRM_OPENGL,
	PRM_SOFTWARE,
	PRM_LAST
};

class PeasantRenderModuleInfo : public PeasantModuleInfo {
	protected:
		PeasantRenderModuleType render_type;
	public:
		PeasantRenderModuleInfo(PeasantRenderModuleType render_type
		                       ,PeasantModuleType type
		                       ,std::string name
		                       ,std::string description
		                       ,std::string version) : PeasantModuleInfo(type,name,description,version) {
			this->render_type = render_type;
		}
};

class PeasantRenderModuleInterface : public PeasantGenericModuleInterface {
	public:
		virtual ~PeasantRenderModuleInterface() {};
	public:
		virtual void render() = 0;
		virtual void redraw() = 0;
		virtual void flip() = 0;
};


typedef PeasantRenderModuleInterface *peasant_render_module_construct();
typedef void peasant_render_module_deconstruct(PeasantRenderModuleInterface *);
typedef PeasantModuleInfo peasant_render_module_info();

#endif
