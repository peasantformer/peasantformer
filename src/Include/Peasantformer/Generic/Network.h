#ifndef PEASANTFORMER_Generic_Network
#define PEASANTFORMER_Generic_Network

#ifdef _WIN32

#define _WIN32_WINNT 0x501
#include <winsock2.h>
#include <ws2tcpip.h>

#else

#include <sys/types.h>
#include <sys/socket.h>
#include <netdb.h>
#include <arpa/inet.h>
#include <unistd.h>

#endif


#endif
