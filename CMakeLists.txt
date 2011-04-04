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
	src/main.cpp
	src/Core/Core.cpp
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

set_directory_properties(PROPERTIES ADDITIONAL_MAKE_CLEAN_FILES modules)



PROJECT(core)
set(OUTPUT_PATH modules/${PROJECT_NAME})
add_custom_target(${PROJECT_NAME} ALL mkdir -p ${OUTPUT_PATH})

PROJECT(engine)
set(LIBRARY_OUTPUT_PATH modules/core)
add_library(engine MODULE src/Core/CoreModules/Engine.cpp)
set_target_properties(engine PROPERTIES PREFIX "")

#add_library(${PROJECT_NAME} MODULE src/Objects/Engine/Engine.cpp)
#add_custom_command(TARGET ${PROJECT_NAME} PRE_BUILD COMMAND mkdir -p modules/${PROJECT_NAME})

