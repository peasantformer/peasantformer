#include <stdio.h>
#include "../../../Interfaces/ObjectInterface.h"

class PeasantObjectSquare : public ObjectInterface {
	public:
};


extern "C" PeasantGenericModuleInfo info() {
	return PeasantGenericModuleInfo(PM_OBJECT,"square","Square object. WTF?","1.0","Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" PeasantObjectModuleInfo object_info() {
	return PeasantObjectModuleInfo(POM_OBJECT);
}

extern "C" PeasantObjectSquare *construct() {
	return new PeasantObjectSquare;
}

extern "C" void deconstruct(PeasantObjectSquare *m) {
	delete m;
}
