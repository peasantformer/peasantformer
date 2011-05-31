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
add_library(data-pstring             STATIC ../src/Data/PString/PString.cpp)
target_link_libraries(data-pstring
	proto-plainiohighlevel
)
add_library(data-hash               STATIC ../src/Data/Hash/Hash.cpp)
target_link_libraries(data-hash
	data-pstring
)
add_library(data-circus             STATIC ../src/Data/CircularBuffer/CircularBuffer.cpp)
#target_link_libraries(data-circus proto-plainiohighlevel)
add_library(data-connection         STATIC ../src/Data/Connection/Connection.cpp)
#target_link_libraries(data-connection 
#	proto-networkhighlevel
#	data-circus
#)
add_library(data-connectionPending  STATIC ../src/Data/Connection/ConnectionPending.cpp)
#target_link_libraries(data-connectionPending
#	data-connection
#	data-circus
#)
add_library(data-connectionAccepted STATIC ../src/Data/Connection/ConnectionAccepted.cpp)

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
#
add_library(proto-plainiohighlevel STATIC ../src/Proto/PlainIO/PlainIOHighLevel.cpp)
target_link_libraries(proto-plainiohighlevel proto-plainio)

#add_library(proto-messages         STATIC ../src/Proto/Messages/Messages.cpp)

add_library(proto-database-sqlite  STATIC ../src/Proto/Database/DatabaseSqlite.cpp)
set(proto-database-sqlite_libs
	data-pstring
	data-hash
)
if (WIN32)
	add_library(sqlite3_static STATIC IMPORTED)
	set_property(TARGET sqlite3_static PROPERTY IMPORTED_LOCATION ../lib/sqlite/libsqlite3.a)	
	set(proto-database-sqlite_libs ${proto-database-sqlite_libs}
		sqlite3_static
	)
elseif (UNIX)
	set(proto-database-sqlite_libs ${proto-database-sqlite_libs}
		sqlite3
	)
endif (WIN32)
target_link_libraries(proto-database-sqlite ${proto-database-sqlite_libs})
add_library(proto-database         STATIC ../src/Proto/Database/Database.cpp)
target_link_libraries(proto-database
	proto-database-sqlite
)
#add_library(proto-protocol       STATIC ../src/Proto/Protocol/Protocol.cpp)

########################################################################

project(Server)
add_library(server-network           STATIC ../src/Apps/Server/ServerNetwork/ServerNetwork.cpp)
target_link_libraries(server-network
	proto-networkhighlevel
)
#add_library(server-database          STATIC ../src/Apps/Server/Database/Database.cpp)
#add_library(server-world             STATIC ../src/Apps/Server/World/World.cpp)
#target_link_libraries(server-world data-circus)	# we don't really need that :P
add_library(server-connections       STATIC ../src/Apps/Server/ServerConnections/ServerConnections.cpp)
#target_link_libraries(server-connections
#	data-connection
#	data-connectionPending
#	data-connectionAccepted
#)
#add_library(server-connectionServer      STATIC ../src/Apps/Server/ConnectionServer/ConnectionServer.cpp)
#add_library(server-loginServer           STATIC ../src/Apps/Server/LoginServer/LoginServer.cpp)
#add_library(server-worldInteractorServer STATIC ../src/Apps/Server/WorldInteractorServer/WorldInteractorServer.cpp)
#add_library(server-protocol          STATIC ../src/Apps/Server/Protocol/Protocol.cpp)

set(server_lib
	server-network
#	server-connectionServer
#	server-loginServer
#	server-worldInteractorServer
	#server-database
	#server-world
	#server-connections
	#server-protocol
#	server-connections

	#proto-database
	#proto-networkhighlevel
	data-pstring	
	proto-network
#	proto-messages
#	proto-plainiohighlevel
#	proto-networkhighlevel
#	proto-database
	#data-circus
#	data-hash
	
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
	)
endif(WIN32)

add_executable(peasantformer-server ../src/Apps/Server/Server.cpp)
target_link_libraries(peasantformer-server ${server_lib})
set_target_properties(peasantformer-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")

########################################################################


