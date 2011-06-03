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
add_library(data-pstring            STATIC ../src/Data/PString/PString.cpp)
add_library(data-hash               STATIC ../src/Data/Hash/Hash.cpp)
add_library(data-circus             STATIC ../src/Data/CircularBuffer/CircularBuffer.cpp)
add_library(data-connection         STATIC ../src/Data/Connection/Connection.cpp)
target_link_libraries(data-connection 
	data-circus
)
add_library(data-connectionPending  STATIC ../src/Data/Connection/ConnectionPending.cpp)
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
add_library(proto-plainiohighlevel STATIC ../src/Proto/PlainIO/PlainIOHighLevel.cpp)
target_link_libraries(proto-plainiohighlevel proto-plainio)

add_library(proto-networkhighlevel STATIC ../src/Proto/Network/NetworkHighLevel.cpp)
target_link_libraries(proto-networkhighlevel proto-network)
add_library(proto-messages         STATIC ../src/Proto/Messages/Messages.cpp)

########################################################################

project(Server)
add_library(server-database          STATIC ../src/Apps/Server/ServerDatabase/ServerDatabase.cpp)
add_library(server-network           STATIC ../src/Apps/Server/ServerNetwork/ServerNetwork.cpp)
add_library(server-connections       STATIC ../src/Apps/Server/ServerConnections/ServerConnections.cpp)
target_link_libraries(server-connections
	data-connection
	data-connectionPending
	data-connectionAccepted
)

add_library(server-thread-connection      STATIC ../src/Apps/Server/ThreadConnection/ThreadConnection.cpp)
add_library(server-thread-login           STATIC ../src/Apps/Server/ThreadLogin/ThreadLogin.cpp)
add_library(server-thread-adminInput      STATIC ../src/Apps/Server/ThreadAdminInput/ThreadAdminInput.cpp)

set(server_lib
	server-network
	server-database	
	server-connections

	server-thread-connection
	server-thread-login
	server-thread-adminInput

	data-pstring
	data-hash

	proto-messages
	proto-plainio
	proto-plainiohighlevel
	proto-network
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
	)
endif(WIN32)

add_executable(peasantformer-server ../src/Apps/Server/Server.cpp)
target_link_libraries(peasantformer-server ${server_lib})
set_target_properties(peasantformer-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")

########################################################################


