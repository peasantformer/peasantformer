cmake_minimum_required(VERSION 2.8)

PROJECT(peasantformer)

include_directories(${SDL_INCLUDE_DIRS})

option(ENABLE_OPENGL_RENDERER "Enable opengl renderer?" ON)
option(ENABLE_SOFTWARE_RENDERER "Enable software renderer?" ON)

message("      *** options ***")
message("OpenGL renderer           ${ENABLE_OPENGL_RENDERER}")
message("Software renderer         ${ENABLE_SOFTWARE_RENDERER}")
message("")
message("")
message("")
message("*** end of options list ***")
message("")
message("")
message("")

set(peasantformer_SRCS
	src/Core.cpp
	src/Engine/Engine.cpp
	src/Engine/IO/IO.cpp
)


INCLUDE(FindSDL) 
find_package(SDL REQUIRED)
set(LIBS ${LIBS} ${SDL_LIBRARY})

if (ENABLE_SOFTWARE_RENDERER)
		add_definitions(-DENABLE_SOFTWARE_RENDERER)
endif(ENABLE_SOFTWARE_RENDERER)

if (ENABLE_OPENGL_RENDERER)
		include(FindOpenGL)
		find_package(OpenGL REQUIRED)
		add_definitions(-DENABLE_OPENGL_RENDERER -g)
		set(LIBS ${LIBS} ${OPENGL_LIBRARIES})
endif(ENABLE_OPENGL_RENDERER)	

ADD_EXECUTABLE(peasantformer ${peasantformer_SRCS})
target_link_libraries(peasantformer ${LIBS})

PROJECT(modules_drectory)
add_custom_target(modules_dir ALL mkdir -p modules)
set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES modules)

PROJECT(modules_engine)
add_custom_target(modules_engine_dir ALL mkdir -p modules/engine)
set(LIBRARY_OUTPUT_PATH modules/engine)
add_library(test MODULE src/Modules/Test/Test.cpp)
set_target_properties(test PROPERTIES PREFIX "")

#add_library(engine MODULE src/main.cpp)
#set_target_properties(engine PROPERTIES PREFIX "")


#PROJECT(modules_objects)
#add_custom_target(modules_objects_dir ALL mkdir -p modules/objects)
#set(LIBRARY_OUTPUT_PATH modules/objects)


#set(LIBRARY_OUTPUT_PATH modules/core)
#add_library(engine MODULE src/Core/CoreModules/Engine.cpp)
#set_target_properties(engine PROPERTIES PREFIX "")

