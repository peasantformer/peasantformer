#ifndef PEASANTFORMER_RENDER_MODULE_INTERFACE
#define PEASANTFORMER_RENDER_MODULE_INTERFACE


#include <stdio.h>

#include <map>
#include <string>

#include "GenericModuleInterface.h"
#include "Types.h"


class PeasantRenderModuleInterface : public PeasantGenericModuleInterface {
	public:
		virtual ~PeasantRenderModuleInterface() {};
	public:
		virtual void render() = 0;
		virtual void redraw() = 0;
		virtual void flip() = 0;
};


enum PeasantRenderModuleType {
	PRM_INVALID,
	PRM_HARDWARE,
	PRM_SOFTWARE,
	PRM_LAST
};

class PeasantRenderModuleInfo {
	protected:
		PeasantRenderModuleType render_type;
	public:
		PeasantRenderModuleInfo(PeasantRenderModuleType render_type) {
			this->render_type = render_type;
		}
	public:
		PeasantRenderModuleType get_render_type(void) {
			return this->render_type;
		}

};

typedef PeasantRenderModuleInterface *peasant_render_module_construct();
typedef void peasant_render_module_deconstruct(PeasantRenderModuleInterface *);
typedef PeasantRenderModuleInfo peasant_render_module_info();


class PeasantRenderModule : public PeasantGenericModule {
	private:
		PeasantRenderModuleType render_type;
	public:
		PeasantRenderModule() {
			this->render_type = PRM_INVALID;
		}
	public:
		PeasantRenderModule(PeasantRenderModuleType render_type
		                   ,std::string filepath
		                   ,std::string name
		                   ,std::string description
		                   ,std::string version
		                   ,std::string author
		                   ,peasant_module_construct *constructor
		                   ,peasant_module_deconstruct *deconstructor
		                   ) : PeasantGenericModule(PM_RENDER,filepath,name,description,version,author,constructor,deconstructor) {
			this->render_type = render_type;
		}

	public:
		PeasantRenderModuleType get_render_type(void) {
			return this->render_type;
		}

};



class PeasantRenderModules {
	private:
		std::map<std::string, PeasantRenderModule> renders;
	
		std::map<std::string, PeasantRenderModule> hardware_renders;
		std::map<std::string, PeasantRenderModule> software_renders;
	public:
		PeasantRenderModules() {
		}
	public:
		int add_render(PeasantRenderModule m) {
			this->renders[m.get_name()] = m;
			switch(m.get_render_type()) {
				case PRM_INVALID:
					return -1;
					break;
				case PRM_HARDWARE:
					this->hardware_renders[m.get_name()] = m;
					break;
				case PRM_SOFTWARE:
					this->software_renders[m.get_name()] = m;
					break;
			}
		}
	public:
		void print() {
			printf("Following Render modules were found:\n");
			printf("  Hardware renders:\n");
			if (this->hardware_renders.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string,PeasantRenderModule>::iterator it = this->hardware_renders.begin(); it != this->hardware_renders.end(); it++) {
				PeasantRenderModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
			printf("  Software renders:\n");
			if (this->software_renders.size() == 0)
				printf("    <None>\n");	
			for (std::map<std::string, PeasantRenderModule>::iterator it = this->software_renders.begin(); it != this->software_renders.end(); it++) {
				PeasantRenderModule m = it->second;
				printf("    * %s %s - %s\n",m.get_name().c_str(),m.get_version().c_str(),m.get_description().c_str());
			}
		}
		PeasantRenderModule get_render_by_name(std::string name) {
			std::map<std::string, PeasantRenderModule>::iterator it;
			
			it = this->renders.find(name);
			if (it != this->renders.end())
				return it->second;
			return PeasantRenderModule();
		}
};
#endif
