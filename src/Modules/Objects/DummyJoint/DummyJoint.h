#ifndef PEASANTFORMER_OBJECT_MODULE_DUMMYJOINT
#define PEASANTFORMER_OBJECT_MODULE_DUMMYJOINT

#include <stdio.h>
#include "../../../Interfaces/ObjectInterface.h"

class PeasantJointDummy : public JointInterface {
	public:
};


extern "C" PeasantGenericModuleInfo info() {
	return PeasantGenericModuleInfo(
		 PM_OBJECT
		,"dummyjoint"
		,"Simple joint not applying any constraints."
		,"1.0"
		,"Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" PeasantObjectModuleInfo object_info() {
	return PeasantObjectModuleInfo(POM_JOINT);
}

extern "C" PeasantJointDummy *construct() {
	return new PeasantJointDummy;
}

extern "C" void deconstruct(PeasantJointDummy *m) {
	delete m;
}

#endif
