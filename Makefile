all:
	cd build-unix; cmake .; make
	cd build-windows; cmake .; make
	
clean:
	cd build-unix; make clean;
	cd build-windows; make clean;
	cd build-windows; rm -fr CMakeCache.txt CMakeFiles cmake_install.cmake Makefile
	cd build-unix; rm -fr CMakeCache.txt CMakeFiles cmake_install.cmake Makefile


depclean: clean
	rm -r modules
