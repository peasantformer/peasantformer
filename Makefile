DEVICE=192.168.1.10:5555
ACTIVITY=net.neverb.peasantformer/.PeasantFormer
INTENT=android.intent.action.MAIN
APK=bin/PeasantFormer-debug.apk
ADB=adb
ANT=ant
DOXYGEN=doxygen

all: ant-compile

debug: ant-clean ant-compile test

test: run logcat

run:
	$(ADB) -s $(DEVICE) install -r $(APK)
#	sleep 1
	$(ADB) -s $(DEVICE) shell am start -a $(INTENT) -n $(ACTIVITY)

logcat:
	$(ADB) -s $(DEVICE) logcat | ./colorify
	$(ADB) -s $(DEVICE) logcat -c

clean: ant-clean doxy-clean

ant-clean:
	$(ANT) clean

ant-compile:
	$(MAKE) -C src/net/neverb/peasantformer/interfaces/objects/parser/mouse
	$(ANT) debug

doxy-docs:
	$(DOXYGEN) docs/Doxyfile

doxy-clean:
	$(RM) -r docs/html/*



.PHONY: all install run clean ant-clean ant-compile doxy-docs doxy-clean
