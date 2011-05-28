all: native docs

docs: $(shell find src build-native -type f) docs/doxygen-config
	doxygen docs/doxygen-config

native: 
	cd build-native; cmake .; make;

cross-win32:
	# Yes, we need that twice for some reason. Otherwise - -static-flags
	# will not be included.
	cd build-cross-win32; cmake .; cmake .; make

clean:
	cd build-native; make clean; true
	cd build-cross-win32; make clean; true
	cd build-cross-win32; rm -fr CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
	cd build-native; rm -fr CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
	rm -rf modules CMakeFiles CMakeCache.txt
	rm -rf docs/html

commit:
	git add docs src
	git commit -a
	git push
