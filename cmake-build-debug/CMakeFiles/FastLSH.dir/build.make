# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.6

# Delete rule output on recipe failure.
.DELETE_ON_ERROR:


#=============================================================================
# Special targets provided by cmake.

# Disable implicit rules so canonical targets will work.
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
CMAKE_COMMAND = /home/peter/clion-2016.3.2/bin/cmake/bin/cmake

# The command to remove a file.
RM = /home/peter/clion-2016.3.2/bin/cmake/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/peter/FYP/FastLSH

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/peter/FYP/FastLSH/cmake-build-debug

# Include any dependencies generated for this target.
include CMakeFiles/FastLSH.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/FastLSH.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/FastLSH.dir/flags.make

CMakeFiles/FastLSH.dir/tests/main.cpp.o: CMakeFiles/FastLSH.dir/flags.make
CMakeFiles/FastLSH.dir/tests/main.cpp.o: ../tests/main.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/peter/FYP/FastLSH/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/FastLSH.dir/tests/main.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/FastLSH.dir/tests/main.cpp.o -c /home/peter/FYP/FastLSH/tests/main.cpp

CMakeFiles/FastLSH.dir/tests/main.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/FastLSH.dir/tests/main.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/peter/FYP/FastLSH/tests/main.cpp > CMakeFiles/FastLSH.dir/tests/main.cpp.i

CMakeFiles/FastLSH.dir/tests/main.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/FastLSH.dir/tests/main.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/peter/FYP/FastLSH/tests/main.cpp -o CMakeFiles/FastLSH.dir/tests/main.cpp.s

CMakeFiles/FastLSH.dir/tests/main.cpp.o.requires:

.PHONY : CMakeFiles/FastLSH.dir/tests/main.cpp.o.requires

CMakeFiles/FastLSH.dir/tests/main.cpp.o.provides: CMakeFiles/FastLSH.dir/tests/main.cpp.o.requires
	$(MAKE) -f CMakeFiles/FastLSH.dir/build.make CMakeFiles/FastLSH.dir/tests/main.cpp.o.provides.build
.PHONY : CMakeFiles/FastLSH.dir/tests/main.cpp.o.provides

CMakeFiles/FastLSH.dir/tests/main.cpp.o.provides.build: CMakeFiles/FastLSH.dir/tests/main.cpp.o


# Object files for target FastLSH
FastLSH_OBJECTS = \
"CMakeFiles/FastLSH.dir/tests/main.cpp.o"

# External object files for target FastLSH
FastLSH_EXTERNAL_OBJECTS =

FastLSH: CMakeFiles/FastLSH.dir/tests/main.cpp.o
FastLSH: CMakeFiles/FastLSH.dir/build.make
FastLSH: CMakeFiles/FastLSH.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/peter/FYP/FastLSH/cmake-build-debug/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable FastLSH"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/FastLSH.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/FastLSH.dir/build: FastLSH

.PHONY : CMakeFiles/FastLSH.dir/build

CMakeFiles/FastLSH.dir/requires: CMakeFiles/FastLSH.dir/tests/main.cpp.o.requires

.PHONY : CMakeFiles/FastLSH.dir/requires

CMakeFiles/FastLSH.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/FastLSH.dir/cmake_clean.cmake
.PHONY : CMakeFiles/FastLSH.dir/clean

CMakeFiles/FastLSH.dir/depend:
	cd /home/peter/FYP/FastLSH/cmake-build-debug && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/peter/FYP/FastLSH /home/peter/FYP/FastLSH /home/peter/FYP/FastLSH/cmake-build-debug /home/peter/FYP/FastLSH/cmake-build-debug /home/peter/FYP/FastLSH/cmake-build-debug/CMakeFiles/FastLSH.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/FastLSH.dir/depend

