# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.25

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:

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
CMAKE_SOURCE_DIR = /home/alan/Desktop/LatencyBench-FastDDS

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/alan/Desktop/LatencyBench-FastDDS/build

# Include any dependencies generated for this target.
include CMakeFiles/HelloWorld_lib.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/HelloWorld_lib.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/HelloWorld_lib.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/HelloWorld_lib.dir/flags.make

CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o: CMakeFiles/HelloWorld_lib.dir/flags.make
CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o: /home/alan/Desktop/LatencyBench-FastDDS/HelloWorld.cxx
CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o: CMakeFiles/HelloWorld_lib.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/alan/Desktop/LatencyBench-FastDDS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o -MF CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o.d -o CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o -c /home/alan/Desktop/LatencyBench-FastDDS/HelloWorld.cxx

CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/alan/Desktop/LatencyBench-FastDDS/HelloWorld.cxx > CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.i

CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/alan/Desktop/LatencyBench-FastDDS/HelloWorld.cxx -o CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.s

# Object files for target HelloWorld_lib
HelloWorld_lib_OBJECTS = \
"CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o"

# External object files for target HelloWorld_lib
HelloWorld_lib_EXTERNAL_OBJECTS =

libHelloWorld_lib.a: CMakeFiles/HelloWorld_lib.dir/HelloWorld.cxx.o
libHelloWorld_lib.a: CMakeFiles/HelloWorld_lib.dir/build.make
libHelloWorld_lib.a: CMakeFiles/HelloWorld_lib.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/alan/Desktop/LatencyBench-FastDDS/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX static library libHelloWorld_lib.a"
	$(CMAKE_COMMAND) -P CMakeFiles/HelloWorld_lib.dir/cmake_clean_target.cmake
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/HelloWorld_lib.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/HelloWorld_lib.dir/build: libHelloWorld_lib.a
.PHONY : CMakeFiles/HelloWorld_lib.dir/build

CMakeFiles/HelloWorld_lib.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/HelloWorld_lib.dir/cmake_clean.cmake
.PHONY : CMakeFiles/HelloWorld_lib.dir/clean

CMakeFiles/HelloWorld_lib.dir/depend:
	cd /home/alan/Desktop/LatencyBench-FastDDS/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/alan/Desktop/LatencyBench-FastDDS /home/alan/Desktop/LatencyBench-FastDDS /home/alan/Desktop/LatencyBench-FastDDS/build /home/alan/Desktop/LatencyBench-FastDDS/build /home/alan/Desktop/LatencyBench-FastDDS/build/CMakeFiles/HelloWorld_lib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/HelloWorld_lib.dir/depend

