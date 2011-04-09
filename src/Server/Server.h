#ifndef PEASANTOFORMER_Server_Main
#define PEASANTOFORMER_Server_Main

#ifdef _WIN32
#define CLOSE_SOCKET(SOCK) closesocket(SOCK)
#else
#define CLOSE_SOCKET(SOCK) close(SOCK)
#endif

#include <sys/types.h>
#include <sys/socket.h>
#include <sys/time.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <unistd.h>

#include <stdio.h>
#include <string.h>

#include <string>


#include <Peasantformer/Protocol.h>

#endif