# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.5

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
CMAKE_COMMAND = /usr/bin/cmake

# The command to remove a file.
RM = /usr/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/peter/FYP/FastLSH

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/peter/FYP/FastLSH

# Include any dependencies generated for this target.
include CMakeFiles/paraGen.dir/depend.make

# Include the progress variables for this target.
include CMakeFiles/paraGen.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/paraGen.dir/flags.make

CMakeFiles/paraGen.dir/src/paragen.cpp.o: CMakeFiles/paraGen.dir/flags.make
CMakeFiles/paraGen.dir/src/paragen.cpp.o: src/paragen.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/peter/FYP/FastLSH/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/paraGen.dir/src/paragen.cpp.o"
	/usr/bin/c++   $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/paraGen.dir/src/paragen.cpp.o -c /home/peter/FYP/FastLSH/src/paragen.cpp

CMakeFiles/paraGen.dir/src/paragen.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/paraGen.dir/src/paragen.cpp.i"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/peter/FYP/FastLSH/src/paragen.cpp > CMakeFiles/paraGen.dir/src/paragen.cpp.i

CMakeFiles/paraGen.dir/src/paragen.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/paraGen.dir/src/paragen.cpp.s"
	/usr/bin/c++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/peter/FYP/FastLSH/src/paragen.cpp -o CMakeFiles/paraGen.dir/src/paragen.cpp.s

CMakeFiles/paraGen.dir/src/paragen.cpp.o.requires:

.PHONY : CMakeFiles/paraGen.dir/src/paragen.cpp.o.requires

CMakeFiles/paraGen.dir/src/paragen.cpp.o.provides: CMakeFiles/paraGen.dir/src/paragen.cpp.o.requires
	$(MAKE) -f CMakeFiles/paraGen.dir/build.make CMakeFiles/paraGen.dir/src/paragen.cpp.o.provides.build
.PHONY : CMakeFiles/paraGen.dir/src/paragen.cpp.o.provides

CMakeFiles/paraGen.dir/src/paragen.cpp.o.provides.build: CMakeFiles/paraGen.dir/src/paragen.cpp.o


# Object files for target paraGen
paraGen_OBJECTS = \
"CMakeFiles/paraGen.dir/src/paragen.cpp.o"

# External object files for target paraGen
paraGen_EXTERNAL_OBJECTS =

paraGen: CMakeFiles/paraGen.dir/src/paragen.cpp.o
paraGen: CMakeFiles/paraGen.dir/build.make
paraGen: CMakeFiles/paraGen.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/peter/FYP/FastLSH/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable paraGen"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/paraGen.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/paraGen.dir/build: paraGen

.PHONY : CMakeFiles/paraGen.dir/build

CMakeFiles/paraGen.dir/requires: CMakeFiles/paraGen.dir/src/paragen.cpp.o.requires

.PHONY : CMakeFiles/paraGen.dir/requires

CMakeFiles/paraGen.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/paraGen.dir/cmake_clean.cmake
.PHONY : CMakeFiles/paraGen.dir/clean

CMakeFiles/paraGen.dir/depend:
	cd /home/peter/FYP/FastLSH && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/peter/FYP/FastLSH /home/peter/FYP/FastLSH /home/peter/FYP/FastLSH /home/peter/FYP/FastLSH /home/peter/FYP/FastLSH/CMakeFiles/paraGen.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/paraGen.dir/depend

