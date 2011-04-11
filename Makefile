all:
	cd build-unix; cmake .; make
	cd build-windows; cmake .; make
	
clean:
	cd build-unix; make clean
	cd build-windows; make clean
