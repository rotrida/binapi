# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.27

# Default target executed when no arguments are given to make.
default_target: all
.PHONY : default_target

# Allow only one "make -f Makefile2" at a time, but pass parallelism.
.NOTPARALLEL:

#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
.SUFFIXES:

# Disable VCS-based implicit rules.
% : %,v

# Disable VCS-based implicit rules.
% : RCS/%

# Disable VCS-based implicit rules.
% : RCS/%,v

# Disable VCS-based implicit rules.
% : SCCS/s.%

# Disable VCS-based implicit rules.
% : s.%

.SUFFIXES: .hpux_make_needs_suffix_list

# Command-line flag to silence nested $(MAKE).
$(VERBOSE)MAKESILENT = -s

#Suppress display of executed commands.
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
RM = /usr/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/rodrigo/projects/strategies/external/binapi

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/rodrigo/projects/strategies/external/binapi

#=============================================================================
# Targets provided globally by CMake.

# Special rule for the target edit_cache
edit_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "No interactive CMake dialog available..."
	/usr/bin/cmake -E echo No\ interactive\ CMake\ dialog\ available.
.PHONY : edit_cache

# Special rule for the target edit_cache
edit_cache/fast: edit_cache
.PHONY : edit_cache/fast

# Special rule for the target rebuild_cache
rebuild_cache:
	@$(CMAKE_COMMAND) -E cmake_echo_color "--switch=$(COLOR)" --cyan "Running CMake to regenerate build system..."
	/usr/bin/cmake --regenerate-during-build -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR)
.PHONY : rebuild_cache

# Special rule for the target rebuild_cache
rebuild_cache/fast: rebuild_cache
.PHONY : rebuild_cache/fast

# The main all target
all: cmake_check_build_system
	$(CMAKE_COMMAND) -E cmake_progress_start /home/rodrigo/projects/strategies/external/binapi/CMakeFiles /home/rodrigo/projects/strategies/external/binapi//CMakeFiles/progress.marks
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 all
	$(CMAKE_COMMAND) -E cmake_progress_start /home/rodrigo/projects/strategies/external/binapi/CMakeFiles 0
.PHONY : all

# The main clean target
clean:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 clean
.PHONY : clean

# The main clean target
clean/fast: clean
.PHONY : clean/fast

# Prepare targets for installation.
preinstall: all
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall

# Prepare targets for installation.
preinstall/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 preinstall
.PHONY : preinstall/fast

# clear depends
depend:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 1
.PHONY : depend

#=============================================================================
# Target rules for targets named binapi

# Build rule for target.
binapi: cmake_check_build_system
	$(MAKE) $(MAKESILENT) -f CMakeFiles/Makefile2 binapi
.PHONY : binapi

# fast build rule for target.
binapi/fast:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/build
.PHONY : binapi/fast

src/api.o: src/api.cpp.o
.PHONY : src/api.o

# target to build an object file
src/api.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/api.cpp.o
.PHONY : src/api.cpp.o

src/api.i: src/api.cpp.i
.PHONY : src/api.i

# target to preprocess a source file
src/api.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/api.cpp.i
.PHONY : src/api.cpp.i

src/api.s: src/api.cpp.s
.PHONY : src/api.s

# target to generate assembly for a file
src/api.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/api.cpp.s
.PHONY : src/api.cpp.s

src/enums.o: src/enums.cpp.o
.PHONY : src/enums.o

# target to build an object file
src/enums.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/enums.cpp.o
.PHONY : src/enums.cpp.o

src/enums.i: src/enums.cpp.i
.PHONY : src/enums.i

# target to preprocess a source file
src/enums.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/enums.cpp.i
.PHONY : src/enums.cpp.i

src/enums.s: src/enums.cpp.s
.PHONY : src/enums.s

# target to generate assembly for a file
src/enums.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/enums.cpp.s
.PHONY : src/enums.cpp.s

src/errors.o: src/errors.cpp.o
.PHONY : src/errors.o

# target to build an object file
src/errors.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/errors.cpp.o
.PHONY : src/errors.cpp.o

src/errors.i: src/errors.cpp.i
.PHONY : src/errors.i

# target to preprocess a source file
src/errors.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/errors.cpp.i
.PHONY : src/errors.cpp.i

src/errors.s: src/errors.cpp.s
.PHONY : src/errors.s

# target to generate assembly for a file
src/errors.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/errors.cpp.s
.PHONY : src/errors.cpp.s

src/pairslist.o: src/pairslist.cpp.o
.PHONY : src/pairslist.o

# target to build an object file
src/pairslist.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/pairslist.cpp.o
.PHONY : src/pairslist.cpp.o

src/pairslist.i: src/pairslist.cpp.i
.PHONY : src/pairslist.i

# target to preprocess a source file
src/pairslist.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/pairslist.cpp.i
.PHONY : src/pairslist.cpp.i

src/pairslist.s: src/pairslist.cpp.s
.PHONY : src/pairslist.s

# target to generate assembly for a file
src/pairslist.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/pairslist.cpp.s
.PHONY : src/pairslist.cpp.s

src/reports.o: src/reports.cpp.o
.PHONY : src/reports.o

# target to build an object file
src/reports.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/reports.cpp.o
.PHONY : src/reports.cpp.o

src/reports.i: src/reports.cpp.i
.PHONY : src/reports.i

# target to preprocess a source file
src/reports.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/reports.cpp.i
.PHONY : src/reports.cpp.i

src/reports.s: src/reports.cpp.s
.PHONY : src/reports.s

# target to generate assembly for a file
src/reports.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/reports.cpp.s
.PHONY : src/reports.cpp.s

src/tools.o: src/tools.cpp.o
.PHONY : src/tools.o

# target to build an object file
src/tools.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/tools.cpp.o
.PHONY : src/tools.cpp.o

src/tools.i: src/tools.cpp.i
.PHONY : src/tools.i

# target to preprocess a source file
src/tools.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/tools.cpp.i
.PHONY : src/tools.cpp.i

src/tools.s: src/tools.cpp.s
.PHONY : src/tools.s

# target to generate assembly for a file
src/tools.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/tools.cpp.s
.PHONY : src/tools.cpp.s

src/types.o: src/types.cpp.o
.PHONY : src/types.o

# target to build an object file
src/types.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/types.cpp.o
.PHONY : src/types.cpp.o

src/types.i: src/types.cpp.i
.PHONY : src/types.i

# target to preprocess a source file
src/types.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/types.cpp.i
.PHONY : src/types.cpp.i

src/types.s: src/types.cpp.s
.PHONY : src/types.s

# target to generate assembly for a file
src/types.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/types.cpp.s
.PHONY : src/types.cpp.s

src/websocket.o: src/websocket.cpp.o
.PHONY : src/websocket.o

# target to build an object file
src/websocket.cpp.o:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/websocket.cpp.o
.PHONY : src/websocket.cpp.o

src/websocket.i: src/websocket.cpp.i
.PHONY : src/websocket.i

# target to preprocess a source file
src/websocket.cpp.i:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/websocket.cpp.i
.PHONY : src/websocket.cpp.i

src/websocket.s: src/websocket.cpp.s
.PHONY : src/websocket.s

# target to generate assembly for a file
src/websocket.cpp.s:
	$(MAKE) $(MAKESILENT) -f CMakeFiles/binapi.dir/build.make CMakeFiles/binapi.dir/src/websocket.cpp.s
.PHONY : src/websocket.cpp.s

# Help Target
help:
	@echo "The following are some of the valid targets for this Makefile:"
	@echo "... all (the default if no target is provided)"
	@echo "... clean"
	@echo "... depend"
	@echo "... edit_cache"
	@echo "... rebuild_cache"
	@echo "... binapi"
	@echo "... src/api.o"
	@echo "... src/api.i"
	@echo "... src/api.s"
	@echo "... src/enums.o"
	@echo "... src/enums.i"
	@echo "... src/enums.s"
	@echo "... src/errors.o"
	@echo "... src/errors.i"
	@echo "... src/errors.s"
	@echo "... src/pairslist.o"
	@echo "... src/pairslist.i"
	@echo "... src/pairslist.s"
	@echo "... src/reports.o"
	@echo "... src/reports.i"
	@echo "... src/reports.s"
	@echo "... src/tools.o"
	@echo "... src/tools.i"
	@echo "... src/tools.s"
	@echo "... src/types.o"
	@echo "... src/types.i"
	@echo "... src/types.s"
	@echo "... src/websocket.o"
	@echo "... src/websocket.i"
	@echo "... src/websocket.s"
.PHONY : help



#=============================================================================
# Special targets to cleanup operation of make.

# Special rule to run CMake to check the build system integrity.
# No rule that depends on this can have commands that come from listfiles
# because they might be regenerated.
cmake_check_build_system:
	$(CMAKE_COMMAND) -S$(CMAKE_SOURCE_DIR) -B$(CMAKE_BINARY_DIR) --check-build-system CMakeFiles/Makefile.cmake 0
.PHONY : cmake_check_build_system

