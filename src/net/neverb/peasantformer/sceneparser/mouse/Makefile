MOUSEJAR=../../../../../../libs/Mouse-1.4.jar
CLASSNAME=MouseParser
PARSERTXT=MouseRules.txt
SYMANTICS=$(CLASSNAME)Symantics
PACKAGE=net.neverb.peasantformer.sceneparser.mouse

all:
	java -cp $(MOUSEJAR) \
		mouse.Generate \
			-G $(PARSERTXT) \
			-P $(CLASSNAME) \
			-S $(SYMANTICS) \
			-p $(PACKAGE)
