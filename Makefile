DEVICE=192.168.1.10:5555
ACTIVITY=net.neverb.peasantformer/.PeasantFormer
INTENT=android.intent.action.MAIN
APK=bin/PeasantFormer-debug.apk
ADB=adb
ANT=ant
DOXYGEN=doxygen

all: ant-compile

debug: ant-clean ant-compile doxy-clean doxy-docs install run

install:
	$(ADB) -s $(DEVICE)  install -r $(APK)

run:
	$(ADB) -s $(DEVICE) shell am start -a $(INTENT) -n $(ACTIVITY)

clean: ant-clean doxy-clean

ant-clean:
	$(ANT) clean

ant-compile:
	$(ANT) debug

doxy-docs:
	$(DOXYGEN) docs/Doxyfile

doxy-clean:
	$(RM) -r docs/html/*



.PHONY: all install run clean ant-clean ant-compile doxy-docs doxy-clean
