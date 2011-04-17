set(CMAKE_SYSTEM_NAME Windows)
set(CMAKE_SYSTEM_VERSION 1)
set(CMAKE_C_COMPILER i686-mingw32-gcc)
set(CMAKE_CXX_COMPILER i686-mingw32-g++)
set(CMAKE_FIND_ROOT_PATH /usr/i686-mingw32)

set(CMAKE_CXX_FLAGS ${CMAKE_CXX_FLAGS} "-static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_DEBUG "-g -static-libgcc -static-libstdc++")
set(CMAKE_CXX_FLAGS_RELEASE ${CMAKE_CXX_FLAGS_RELEASE} "-static-libgcc -static-libstdc++")
set(CMAKE_MODULE_LINKER_FLAGS "-static-libgcc -static-libstdc++")
#set(CMAKE_FIND_ROOT_PATH_MODE_PROGRAM ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_LIBRARY ONLY)
#set(CMAKE_FIND_ROOT_PATH_MODE_INCLUDE ONLY)