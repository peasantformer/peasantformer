#ifndef PEASANTFORMER_OBJECT_MODULE_RECTANGLE
#define PEASANTFORMER_OBJECT_MODULE_RECTANGLE



class PeasantObjectRectangle : public ParticleInterface
                             , public NPCInterface
                             , public PCInterface
                             , public RunnerInterface {
	public:
};


extern "C" PeasantGenericModuleInfo info() {
	return PeasantGenericModuleInfo(PM_OBJECT,"rectangle","Rectangle-shaped object.","1.0","Alexander <itakingiteasy@gmail.com> Tumin");
}
extern "C" PeasantObjectModuleInfo object_info() {
	return PeasantObjectModuleInfo(POM_PARTICLE | POM_RUNNER | POM_PC | POM_NPC);
}

extern "C" PeasantObjectRectangle *construct() {
	return new PeasantObjectRectangle;
}

extern "C" void deconstruct(PeasantObjectRectangle *m) {
	delete m;
}

#endif
