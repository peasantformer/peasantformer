cmake_minimum_required(VERSION 2.8)

########################################################################

set(CMAKE_TOOLCHAIN_FILE platform.cmake)
add_definitions(-pedantic -Wall -g)

########################################################################

set(CMAKE_BUILD_TYPE Debug)
include_directories(
	../src
)

########################################################################

project(Data)
add_library(data-hash           STATIC ../src/Data/Hash/Hash.cpp)

########################################################################

project(Proto)
set(network_src
	../src/Proto/Network/Network.cpp
)
if (WIN32)
	set(network_src ${network_src}
		../src/Proto/Network/NetworkWin32.cpp
	)	
elseif (UNIX)
	set(network_src ${network_src}
		../src/Proto/Network/NetworkUnix.cpp
	)	
endif (WIN32)

add_library(proto-network          STATIC ${network_src})
if (WIN32)
		target_link_libraries(proto-network ws2_32)	
endif (WIN32)


add_library(proto-networkhighlevel STATIC ../src/Proto/Network/NetworkHighLevel.cpp)
target_link_libraries(proto-networkhighlevel proto-network)


set(plainio_src
	../src/Proto/PlainIO/PlainIO.cpp
)
if (WIN32)
	set(plainio_src ${plainio_src}
		../src/Proto/PlainIO/PlainIOWin32.cpp
	)	
elseif (UNIX)
	set(plainio_src ${plainio_src}
		../src/Proto/PlainIO/PlainIOUnix.cpp
	)	
endif (WIN32)
add_library(proto-plainio          STATIC ${plainio_src})

add_library(proto-plainiohighlevel STATIC ../src/Proto/PlainIO/PlainIOHighLevel.cpp)
target_link_libraries(proto-plainiohighlevel proto-plainio)

add_library(proto-messages         STATIC ../src/Proto/Messages/Messages.cpp)

########################################################################

project(Server)
add_library(server-network           STATIC ../src/Apps/Server/Network/Network.cpp)
#add_library(server-database          STATIC ../src/Apps/Server/Database/Database.cpp)
add_library(server-world             STATIC ../src/Apps/Server/World/World.cpp)

set(server_lib
	server-network
	#server-database
	server-world
	
	proto-messages
	proto-plainiohighlevel
	proto-networkhighlevel
)

if (WIN32)
	include_directories(
		../include
	)
	add_definitions(-DPTW32_STATIC_LIB )
	add_library(pthread_static STATIC IMPORTED)
	set_property(TARGET pthread_static PROPERTY IMPORTED_LOCATION ../lib/pthread-w32/libpthread-w32-GC.a)

	set(server_lib ${server_lib}
		pthread_static
	)
elseif (UNIX)
	set(server_lib ${server_lib}
		pthread
		sqlite3
	)
endif(WIN32)

add_executable(peasantformer-server ../src/Apps/Server/Server.cpp)
target_link_libraries(peasantformer-server ${server_lib})
set_target_properties(peasantformer-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")

########################################################################


