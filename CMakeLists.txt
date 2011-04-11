cmake_minimum_required(VERSION 2.8)

set(CLEAN_FILES_LIST
	CMakeCache.txt
	CMakeFiles
	cmake_install.cmake	
	Makefile
)

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES "${CLEAN_FILES_LIST}")

if (WIN32)
	set(CMAKE_C_COMPILER mingw32-gcc)
	set(CMAKE_CXX_COMPILER mingw32-g++)
endif(WIN32)

add_definitions(-g)
include_directories(src/Include)

project(Peasantformer)

find_package(SDL REQUIRED)
set(CLIENTLIBS ${SDL_LIBRARY})

set(Peasantformer_SRCS
	src/Client/Client.cpp
)

if (WIN32)
set(Peasantformer_SRCS
	${Peasantformer_SRCS}
	src/Network/NetworkWin32.cpp
)
set(CLIENTLIBS ${CLIENTLIBS} -lws2_32)
endif(WIN32)

if (UNIX)
set(Peasantformer_SRCS
	${Peasantformer_SRCS}
	src/Network/NetworkUnix.cpp
)
endif(UNIX)

add_executable(peasantformer ${Peasantformer_SRCS})
target_link_libraries(peasantformer ${CLIENTLIBS})






project(Peasantformer-server)
find_package(SDL REQUIRED)
set(SERVERLIBS ${SDL_LIBRARY})

set(Peasantformer-server_SRCS
	src/Server/Server.cpp
)

add_executable(peasantformer-server ${Peasantformer-server_SRCS})
target_link_libraries(peasantformer-server ${SERVERLIBS})
