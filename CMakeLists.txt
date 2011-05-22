cmake_minimum_required(VERSION 2.8)

########################################################################

set(CMAKE_TOOLCHAIN_FILE platform.cmake)
add_definitions(-pedantic -Wall)

########################################################################

set(CMAKE_BUILD_TYPE Debug)
include_directories(
	../src
	/usr/include/Box2D
)

########################################################################

project(Data)
add_library(data-array          STATIC ../src/Data/Array/Array.cpp)
add_library(data-object         STATIC ../src/Data/Object/Object.cpp)
add_library(data-section        STATIC ../src/Data/Section/Section.cpp)
add_library(data-circularbuffer STATIC ../src/Data/CircularBuffer/CircularBuffer.cpp)
add_library(data-particle       STATIC ../src/Data/Particle/Particle.cpp)
add_library(data-joint          STATIC ../src/Data/Joint/Joint.cpp)
add_library(data-vector2f       STATIC ../src/Data/Vector2f/Vector2f.cpp)
add_library(data-world          STATIC ../src/Data/World/World.cpp)


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
add_library(proto-networkhighlevel STATIC ../src/Proto/Network/NetworkHighLevel.cpp)
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

########################################################################

project(Database)
set (database_src ../src/Database/DBInterface/DBInterface.cpp)
set (database_src ${database_src}
		../src/Database/DBInterface/SQLiteInterface.cpp
	)
add_library(database-interface STATIC ${database_src})


########################################################################

#project(PhysTest)
#include(FindOpenGL)
#include(FindSDL)

#set(phystest_src
#	../src/PhysTest/PhysTest.cpp
#)

#set(phystest_lib
#	${OPENGL_LIBRARIES}
#	${SDL_LIBRARY}
#	-lbox2d
#
#	data-vector2f
#	data-array
#)
#
#
#add_executable(phystest ${phystest_src})
#target_link_libraries(phystest ${phystest_lib})
#set_target_properties(phystest PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")

########################################################################

project(Client)

set(client_src
	../src/Client/Client.cpp
)

set(client_lib
	data-circularbuffer
)

add_executable(peasantformer ${client_src})
target_link_libraries(peasantformer ${client_lib})
set_target_properties(peasantformer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")

########################################################################

project(Server)

set(server_src
	../src/Server/Server.cpp
	../src/Server/Network.cpp
)

set(server_lib
	data-circularbuffer
	data-vector2f
	data-joint
	data-particle
	data-object
	data-section
	data-world

	database-interface
	
	proto-network
	proto-networkhighlevel
	proto-plainio
	proto-plainiohighlevel
)
if (WIN32)
	include_directories(
		../include
	)
	add_definitions(-DPTW32_STATIC_LIB )
	add_library(pthread_static STATIC IMPORTED)
	set_property(TARGET pthread_static PROPERTY IMPORTED_LOCATION ../lib/pthread-w32/libpthread-w32-GC.a)

	add_library(sqlite3_static STATIC IMPORTED)
	set_property(TARGET sqlite3_static PROPERTY IMPORTED_LOCATION ../lib/sqlite/sqlite3.dll)
	set(server_lib ${server_lib}
		pthread_static
		sqlite3_static
		ws2_32
	)	
elseif (UNIX)
	set(server_lib ${server_lib}
		pthread
		sqlite3
	)	
endif (WIN32)
add_executable(peasantformer-server ${server_src})
target_link_libraries(peasantformer-server ${server_lib})
set_target_properties(peasantformer-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")

########################################################################



#set(CMAKE_TOOLCHAIN_FILE platform.cmake)

#########################################################################

#set(CMAKE_BUILD_TYPE Debug)
#include_directories(
	#../src/Include
#)

#########################################################################

#########################################################################

#project(Peasantformer-Network)
#set(Peasantformer-Network_SRCS
	#../src/Network/Protocol/Protocol.cpp
	#../src/Network/Net/Net.cpp
	#../src/Network/NetHighLevel/NetHighLevel.cpp
#)

#if (WIN32)
		#set (Peasantformer-Network_SRCS
			#${Peasantformer-Network_SRCS}
			#../src/Network/NetWin32/NetWin32.cpp
		#)
#elseif (UNIX)
		#set (Peasantformer-Network_SRCS
			#${Peasantformer-Network_SRCS}
			#../src/Network/NetUnix/NetUnix.cpp
		#)
#endif (WIN32)
#add_library(peasant-network STATIC ${Peasantformer-Network_SRCS})

#########################################################################

#project(Peasantformer-ModulesEngine)
#set(Peasantformer-ModulesEngine_SRCS
	#../src/Modules/Modules/Modules.cpp
#)
#if (WIN32)
		#set (Peasantformer-ModulesEngine_SRCS
			#${Peasantformer-ModulesEngine_SRCS}
			#../src/Modules/ModulesWin32/ModulesWin32.cpp
		#)
#elseif (UNIX)
		#set (Peasantformer-ModulesEngine_SRCS
			#${Peasantformer-ModulesEngine_SRCS}
			#../src/Modules/ModulesUnix/ModulesUnix.cpp
		#)
#endif (WIN32)
#add_library(peasant-modulesengine STATIC ${Peasantformer-ModulesEngine_SRCS})

#########################################################################

#project(Peasantformer-Client)
#set(Peasantformer-Client_SRCS
	#../src/Client/Client.cpp
#)
#add_executable(peasantformer ${Peasantformer-Client_SRCS})
#set_target_properties(peasantformer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")
#set (Peasantformer-Client_LIBS 
	#peasant-data
	#peasant-network
	#peasant-modulesengine
#)
#if (UNIX)
	#set (Peasantformer-Client_LIBS ${Peasantformer-Client_LIBS} 
		#dl
	#)
#endif(UNIX)
#target_link_libraries(peasantformer ${Peasantformer-Client_LIBS})

#########################################################################

#project(Modules-Renders)
#set (CMAKE_SHARED_MODULE_PREFIX "")
#set(Modules-Renders-OpenGL_SRCS
	#../src/Modules/Renders/RenderOpenGL/RenderOpenGL.cpp
#)
#add_library(opengl MODULE ${Modules-Renders-OpenGL_SRCS})
#set_target_properties(opengl PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../modules/renders")

#########################################################################

#project(Modules-Objects)
#set (CMAKE_SHARED_MODULE_PREFIX "")
#set(Modules-Objects-Square_SRCS
	#../src/Modules/Objects/Square/Square.cpp
#)
#add_library(square MODULE ${Modules-Objects-Square_SRCS})
#set_target_properties(square PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../modules/objects")

#########################################################################

#project(Peasantformer-Server)
#set(Peasantformer-Server_SRCS
	#../src/Server/Server.cpp
	#../src/Server/Network/Network.cpp

#)
#add_executable(peasantformer-server ${Peasantformer-Server_SRCS})
#set_target_properties(peasantformer-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")
#set (Peasantformer-Server_LIBS 
	#peasant-data
	#peasant-network
	#peasant-modulesengine
#)
#if (UNIX)
	#set (Peasantformer-Server_LIBS ${Peasantformer-Server_LIBS} 
		#dl
		#pthread
	#)
#elseif (WIN32)
	#add_definitions(-DPTW32_STATIC_LIB )
	#add_library(imp_pthread STATIC IMPORTED)
	#set_property(TARGET imp_pthread PROPERTY IMPORTED_LOCATION ../src/Libs/PThreads/libpthread.a)
	#set (Peasantformer-Server_LIBS ${Peasantformer-Server_LIBS} 
		#imp_pthread
		#ws2_32
	#)
	#include_directories(
		#../src/Include/Peasantformer/Libs/PThreads/
	#)
#endif(UNIX)
#target_link_libraries(peasantformer-server ${Peasantformer-Server_LIBS})

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
