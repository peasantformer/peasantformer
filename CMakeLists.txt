cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE platform.cmake)

########################################################################

include_directories(../src/Include)

########################################################################

project(Peasantformer-Data)
set(Peasantformer-Data_SRCS
	../src/Data/CircularBuffer/CircularBuffer.cpp
)
add_library(peasant-data STATIC ${Peasantformer-Data_SRCS})

########################################################################

project(Peasantformer-Network)
set(Peasantformer-Network_SRCS
	../src/Network/Protocol/Protocol.cpp
	../src/Network/Net/Net.cpp	
)

if (WIN32)
		set (Peasantformer-Network_SRCS
			${Peasantformer-Network_SRCS}
			../src/Network/NetWin32/NetWin32.cpp
		)
elseif (UNIX)
		set (Peasantformer-Network_SRCS
			${Peasantformer-Network_SRCS}
			../src/Network/NetUnix/NetUnix.cpp
		)
endif (WIN32)
add_library(peasant-network STATIC ${Peasantformer-Network_SRCS})

########################################################################

project(Peasantformer-Client)
set(Peasantformer-Client_SRCS
	../src/Client/Client.cpp
	../src/Client/Network/Network.cpp
)
add_executable(peasantformer ${Peasantformer-Client_SRCS})
target_link_libraries(peasantformer
	peasant-data
	peasant-network
)

########################################################################





#project(Peasantformer-Client)
#find_package(SDL REQUIRED)
#set(CLIENTLIBS ${SDL_LIBRARY})

#set(Peasantformer_SRCS
	#../src/Client/Client.cpp
#)

#if (WIN32)
	#set(CLIENTLIBS ${CLIENTLIBS} -lws2_32)
#elseif(UNIX)	
#else(WIN32)
	#message(FATAL_ERROR "Unknown operation system")
#endif(WIN32)


#add_executable(peasantformer ${Peasantformer_SRCS})
#target_link_libraries(peasantformer ${CLIENTLIBS})


#set(CLEAN_FILES_LIST
	#CMakeCache.txt
	#CMakeFiles
	#cmake_install.cmake	
	#Makefile
#)

#set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES_LIST}")

#add_definitions(-g)
#include_directories(src/Include)

#project(Peasantformer)

#find_package(SDL REQUIRED)
#set(CLIENTLIBS ${SDL_LIBRARY})

#set(Peasantformer_SRCS
	#src/Client/Client.cpp
#)

#if (WIN32)
	#set(Peasantformer_SRCS
		#${Peasantformer_SRCS}
		#src/Network/NetworkWin32.cpp
	#)
	#set(CLIENTLIBS ${CLIENTLIBS} -lws2_32)
#endif(WIN32)

#if (UNIX)
	#set(Peasantformer_SRCS
		#${Peasantformer_SRCS}
		#src/Network/NetworkUnix.cpp
	#)
#endif(UNIX)

#add_executable(peasantformer ${Peasantformer_SRCS})
#target_link_libraries(peasantformer ${CLIENTLIBS})






#project(Peasantformer-server)
#find_package(SDL REQUIRED)
#set(SERVERLIBS ${SDL_LIBRARY})

#set(Peasantformer-server_SRCS
#	src/Server/Server.cpp
#

#add_executable(peasantformer-server ${Peasantformer-server_SRCS})
#target_link_libraries(peasantformer-server ${SERVERLIBS})
