#include "Server.h"

/// @file

/// Main function.
///
/// \param [in] argc is a number of arguments
/// \param [in] argv is an argument list
/// @return always zero.
int main(int argc, char **argv) {
	CircularBufffer cbuf(4);
	cbuf.write("abcde");
	cbuf.dump();
	return 0;
}
