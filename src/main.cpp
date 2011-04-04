#include "main.h"

int main (int argc, char **argv) {
	PeasantCore core((char*)"modules");
	core.scan_modules();
	core.load_modules();
	
	
	return 0;
}
