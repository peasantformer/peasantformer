cmake_minimum_required(VERSION 2.8)

if (WIN32)
	set(CMAKE_C_COMPILER mingw32-gcc)
	set(CMAKE_CXX_COMPILER mingw32-g++)
endif(WIN32)

add_definitions(-g)

project(Peasantformer)
find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)
set(CLIENTLIBS ${SDL_LIBRARY} ${SDLNET_LIBRARY})

set(Peasantformer_SRCS
	src/Client/Client.cpp
)

add_executable(peasantformer ${Peasantformer_SRCS})
target_link_libraries(peasantformer ${CLIENTLIBS})






project(Peasantformer-server)
find_package(SDL REQUIRED)
find_package(SDL_net REQUIRED)
set(SERVERLIBS ${SDL_LIBRARY} ${SDLNET_LIBRARY})

set(Peasantformer-server_SRCS
	src/Server/Server.cpp
)

add_executable(peasantformer-server ${Peasantformer-server_SRCS})
target_link_libraries(peasantformer-server ${SERVERLIBS})
