cmake_minimum_required(VERSION 2.8)

set(CMAKE_TOOLCHAIN_FILE platform.cmake)

########################################################################

set(CMAKE_BUILD_TYPE Debug)
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
	../src/Network/NetHighLevel/NetHighLevel.cpp
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

project(Peasantformer-ModulesEngine)
set(Peasantformer-ModulesEngine_SRCS
	../src/Modules/Modules/Modules.cpp
)
if (WIN32)
		set (Peasantformer-ModulesEngine_SRCS
			${Peasantformer-ModulesEngine_SRCS}
			../src/Modules/ModulesWin32/ModulesWin32.cpp
		)
elseif (UNIX)
		set (Peasantformer-ModulesEngine_SRCS
			${Peasantformer-ModulesEngine_SRCS}
			../src/Modules/ModulesUnix/ModulesUnix.cpp
		)
endif (WIN32)
add_library(peasant-modulesengine STATIC ${Peasantformer-ModulesEngine_SRCS})

########################################################################

project(Peasantformer-Client)
set(Peasantformer-Client_SRCS
	../src/Client/Client.cpp
)
add_executable(peasantformer ${Peasantformer-Client_SRCS})
set_target_properties(peasantformer PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")
set (Peasantformer-Client_LIBS 
	peasant-data
	peasant-network
	peasant-modulesengine
)
if (UNIX)
	set (Peasantformer-Client_LIBS ${Peasantformer-Client_LIBS} 
		dl
	)
endif(UNIX)
target_link_libraries(peasantformer ${Peasantformer-Client_LIBS})

########################################################################

project(Modules-Renders)
set (CMAKE_SHARED_MODULE_PREFIX "")
set(Modules-Renders-OpenGL_SRCS
	../src/Modules/Renders/RenderOpenGL/RenderOpenGL.cpp
)
add_library(opengl MODULE ${Modules-Renders-OpenGL_SRCS})
set_target_properties(opengl PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../modules/renders")

########################################################################

project(Modules-Objects)
set (CMAKE_SHARED_MODULE_PREFIX "")
set(Modules-Objects-Square_SRCS
	../src/Modules/Objects/Square/Square.cpp
)
add_library(square MODULE ${Modules-Objects-Square_SRCS})
set_target_properties(square PROPERTIES LIBRARY_OUTPUT_DIRECTORY "../modules/objects")

########################################################################

project(Peasantformer-Server)
set(Peasantformer-Server_SRCS
	../src/Server/Server.cpp
	../src/Server/Network/Network.cpp

)
add_executable(peasantformer-server ${Peasantformer-Server_SRCS})
set_target_properties(peasantformer-server PROPERTIES RUNTIME_OUTPUT_DIRECTORY "../")
set (Peasantformer-Server_LIBS 
	peasant-data
	peasant-network
	peasant-modulesengine
)
if (UNIX)
	set (Peasantformer-Server_LIBS ${Peasantformer-Server_LIBS} 
		dl
	)
elseif (WIN32)
	set (Peasantformer-Server_LIBS ${Peasantformer-Server_LIBS} 
		ws2_32
	)
endif(UNIX)
target_link_libraries(peasantformer-server ${Peasantformer-Server_LIBS})

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
