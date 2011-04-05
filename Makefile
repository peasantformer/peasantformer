# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canoncical targets will work.
.SUFFIXES:

# Remove some rules from gmake that .SUFFIXES does not remove.
SUFFIXES =

.SUFFIXES: .hpux_make_needs_suffix_list

# Suppress display of executed commands.
$(VERBOSE).SILENT:

# A target that is always out of date.
cmake_force:
.PHONY : cmake_force

#=============================================================================
# Set environment variables for the build.

# The shell in which to execute make rules.
SHELL = /bin/sh

# The CMake executable.
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/user/soft/mine/peasantformer

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/user/soft/mine/peasantformer

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake cache editor..."
	/usr/bin/ccmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/user/soft/mine/peasantformer/CMakeFiles /home/user/soft/mine/peasantformer/CMakeFiles/progress.marks
	$(MAKE) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/user/soft/mine/peasantformer/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named modules_dir

# Build rule for target.
modules_dir: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 modules_dir
.PHONY : modules_dir

# fast build rule for target.
modules_dir/fast:
	$(MAKE) -f CMakeFiles/modules_dir.dir/build.make CMakeFiles/modules_dir.dir/build
.PHONY : modules_dir/fast

#=============================================================================
# Target rules for targets named modules_engine_dir

# Build rule for target.
modules_engine_dir: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 modules_engine_dir
.PHONY : modules_engine_dir

# fast build rule for target.
modules_engine_dir/fast:
	$(MAKE) -f CMakeFiles/modules_engine_dir.dir/build.make CMakeFiles/modules_engine_dir.dir/build
.PHONY : modules_engine_dir/fast

#=============================================================================
# Target rules for targets named peasantformer

# Build rule for target.
peasantformer: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 peasantformer
.PHONY : peasantformer

# fast build rule for target.
peasantformer/fast:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/build
.PHONY : peasantformer/fast

#=============================================================================
# Target rules for targets named test

# Build rule for target.
test: cmake_check_build_system
	$(MAKE) -f CMakeFiles/Makefile2 test
.PHONY : test

# fast build rule for target.
test/fast:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/build
.PHONY : test/fast

src/Core.o: src/Core.cpp.o
.PHONY : src/Core.o

# target to build an object file
src/Core.cpp.o:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Core.cpp.o
.PHONY : src/Core.cpp.o

src/Core.i: src/Core.cpp.i
.PHONY : src/Core.i

# target to preprocess a source file
src/Core.cpp.i:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Core.cpp.i
.PHONY : src/Core.cpp.i

src/Core.s: src/Core.cpp.s
.PHONY : src/Core.s

# target to generate assembly for a file
src/Core.cpp.s:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Core.cpp.s
.PHONY : src/Core.cpp.s

src/Engine/Engine.o: src/Engine/Engine.cpp.o
.PHONY : src/Engine/Engine.o

# target to build an object file
src/Engine/Engine.cpp.o:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Engine/Engine.cpp.o
.PHONY : src/Engine/Engine.cpp.o

src/Engine/Engine.i: src/Engine/Engine.cpp.i
.PHONY : src/Engine/Engine.i

# target to preprocess a source file
src/Engine/Engine.cpp.i:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Engine/Engine.cpp.i
.PHONY : src/Engine/Engine.cpp.i

src/Engine/Engine.s: src/Engine/Engine.cpp.s
.PHONY : src/Engine/Engine.s

# target to generate assembly for a file
src/Engine/Engine.cpp.s:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Engine/Engine.cpp.s
.PHONY : src/Engine/Engine.cpp.s

src/Engine/IO/IO.o: src/Engine/IO/IO.cpp.o
.PHONY : src/Engine/IO/IO.o

# target to build an object file
src/Engine/IO/IO.cpp.o:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Engine/IO/IO.cpp.o
.PHONY : src/Engine/IO/IO.cpp.o

src/Engine/IO/IO.i: src/Engine/IO/IO.cpp.i
.PHONY : src/Engine/IO/IO.i

# target to preprocess a source file
src/Engine/IO/IO.cpp.i:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Engine/IO/IO.cpp.i
.PHONY : src/Engine/IO/IO.cpp.i

src/Engine/IO/IO.s: src/Engine/IO/IO.cpp.s
.PHONY : src/Engine/IO/IO.s

# target to generate assembly for a file
src/Engine/IO/IO.cpp.s:
	$(MAKE) -f CMakeFiles/peasantformer.dir/build.make CMakeFiles/peasantformer.dir/src/Engine/IO/IO.cpp.s
.PHONY : src/Engine/IO/IO.cpp.s

src/Modules/Test/Test.o: src/Modules/Test/Test.cpp.o
.PHONY : src/Modules/Test/Test.o

# target to build an object file
src/Modules/Test/Test.cpp.o:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/src/Modules/Test/Test.cpp.o
.PHONY : src/Modules/Test/Test.cpp.o

src/Modules/Test/Test.i: src/Modules/Test/Test.cpp.i
.PHONY : src/Modules/Test/Test.i

# target to preprocess a source file
src/Modules/Test/Test.cpp.i:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/src/Modules/Test/Test.cpp.i
.PHONY : src/Modules/Test/Test.cpp.i

src/Modules/Test/Test.s: src/Modules/Test/Test.cpp.s
.PHONY : src/Modules/Test/Test.s

# target to generate assembly for a file
src/Modules/Test/Test.cpp.s:
	$(MAKE) -f CMakeFiles/test.dir/build.make CMakeFiles/test.dir/src/Modules/Test/Test.cpp.s
.PHONY : src/Modules/Test/Test.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... modules_dir"
	@echo "... modules_engine_dir"
	@echo "... peasantformer"
	@echo "... rebuild_cache"
	@echo "... test"
	@echo "... src/Core.o"
	@echo "... src/Core.i"
	@echo "... src/Core.s"
	@echo "... src/Engine/Engine.o"
	@echo "... src/Engine/Engine.i"
	@echo "... src/Engine/Engine.s"
	@echo "... src/Engine/IO/IO.o"
	@echo "... src/Engine/IO/IO.i"
	@echo "... src/Engine/IO/IO.s"
	@echo "... src/Modules/Test/Test.o"
	@echo "... src/Modules/Test/Test.i"
	@echo "... src/Modules/Test/Test.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -H$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

