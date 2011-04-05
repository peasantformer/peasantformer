cmake_minimum_required(VERSION 2.8)

PROJECT(peasantformer)

include_directories(${SDL_INCLUDE_DIRS})

option(ENABLE_TEST_MODULE "Build test module?" ON)
option(ENABLE_OPENGL_RENDER_MODULE "Build opengl render module?" ON)
option(ENABLE_SOFTWARE_RENDER_MODULE "Build software render module?" ON)

message("      *** options ***")
message("Test module               ${ENABLE_TEST_MODULE}")
message("OpenGL renderer module    ${ENABLE_OPENGL_RENDER_MODULE}")
message("Software renderer module  ${ENABLE_SOFTWARE_RENDER_MODULE}")
message("")
message("")
message("")
message("*** end of options list ***")
message("")
message("")
message("")

if (WIN32)
	set(CMAKE_C_COMPILER mingw32-gcc)
	set(CMAKE_CXX_COMPILER mingw32-g++)
endif(WIN32)

set(peasantformer_SRCS
	src/main.cpp
	src/Core.cpp
	src/Engine/Engine.cpp
	src/Engine/Phys/Phys.cpp
	src/Engine/IO/IO.cpp
)


INCLUDE(FindSDL) 
find_package(SDL REQUIRED)
set(LIBS ${LIBS} ${SDL_LIBRARY})

if (ENABLE_OPENGL_RENDER_MODULE)
		include(FindOpenGL)
		find_package(OpenGL REQUIRED)
endif(ENABLE_OPENGL_RENDER_MODULE)	

ADD_EXECUTABLE(peasantformer ${peasantformer_SRCS})
target_link_libraries(peasantformer ${LIBS})

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES modules)

if (ENABLE_OPENGL_RENDER_MODULE)
	add_library(opengl MODULE src/Modules/Render/OpenGL.cpp)
	target_link_libraries(opengl ${OPENGL_LIBRARIES})
	set_target_properties(opengl PROPERTIES PREFIX "" LIBRARY_OUTPUT_DIRECTORY modules/engine/render)
endif(ENABLE_OPENGL_RENDER_MODULE)	


add_library(square MODULE src/Modules/Objects/Objects/Square.cpp)
set_target_properties(square PROPERTIES PREFIX "" LIBRARY_OUTPUT_DIRECTORY modules/objects)
