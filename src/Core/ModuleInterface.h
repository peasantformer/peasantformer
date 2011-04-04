#ifndef PEASANTFORMER_MODULE_INTERFACE
#define PEASANTFORMER_MODULE_INTERFACE

struct ModuleInfo {
	char *name;
	char *descr;
	char *version;
};

class PeasantModuleInterface {
	public:
		virtual ~PeasantModuleInterface() {};
	public:
		virtual void test() = 0;

};

typedef PeasantModuleInterface *create_module_t();
typedef void destroy_module_t(PeasantModuleInterface *);

extern "C" ModuleInfo info();
#endif
