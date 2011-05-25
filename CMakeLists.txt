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
add_library(data-circus         STATIC ../src/Data/CircularBuffer/CircularBuffer.cpp)

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
add_library(proto-database         STATIC ../src/Proto/Database/Database.cpp)
set(proto-database_libs data-hash)
if (WIN32)
	add_library(sqlite3_static STATIC IMPORTED)
	set_property(TARGET sqlite3_static PROPERTY IMPORTED_LOCATION ../lib/sqlite/libsqlite3.a)	
	set(proto-database_libs ${proto-database_libs}
		sqlite3_static
	)
endif (WIN32)
target_link_libraries(proto-database ${proto-database_libs})

########################################################################

project(Server)
add_library(server-network           STATIC ../src/Apps/Server/Network/Network.cpp)
add_library(server-database          STATIC ../src/Apps/Server/Database/Database.cpp)
add_library(server-world             STATIC ../src/Apps/Server/World/World.cpp)
target_link_libraries(server-world data-circus)	
add_library(server-connections       STATIC ../src/Apps/Server/Connections/Connections.cpp)

set(server_lib
	server-network
	server-database
	server-world
	server-connections
	
	proto-messages
	proto-plainiohighlevel
	proto-networkhighlevel
	proto-database
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


