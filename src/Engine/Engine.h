#ifndef PEASANTFORMER_ENGINE
#define PEASANTFORMER_ENGINE

#include <vector>
#include <string>

#include <peasantformer/Types.h>
#include <peasantformer/ObjectInterface.h>
#include <peasantformer/RenderInterface.h>

#include "ModulesCore/ModulesCore.h"

class PeasantEngine {
	private:
		PeasantModulesCore modules;
	public:
		PeasantEngine() {
			this->modules.scan_modules_dirs();
			this->modules.load_modules();	
		}
	
	
};

#endif  
