# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.20

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
CMAKE_COMMAND = /usr/local/bin/cmake

# The command to remove a file.
RM = /usr/local/bin/cmake -E rm -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /home/maruan/project_boids

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /home/maruan/project_boids/build

# Include any dependencies generated for this target.
include CMakeFiles/simulation.t.dir/depend.make
# Include any dependencies generated by the compiler for this target.
include CMakeFiles/simulation.t.dir/compiler_depend.make

# Include the progress variables for this target.
include CMakeFiles/simulation.t.dir/progress.make

# Include the compile flags for this target's objects.
include CMakeFiles/simulation.t.dir/flags.make

CMakeFiles/simulation.t.dir/simulation.test.cpp.o: CMakeFiles/simulation.t.dir/flags.make
CMakeFiles/simulation.t.dir/simulation.test.cpp.o: ../simulation.test.cpp
CMakeFiles/simulation.t.dir/simulation.test.cpp.o: CMakeFiles/simulation.t.dir/compiler_depend.ts
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir=/home/maruan/project_boids/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object CMakeFiles/simulation.t.dir/simulation.test.cpp.o"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -MD -MT CMakeFiles/simulation.t.dir/simulation.test.cpp.o -MF CMakeFiles/simulation.t.dir/simulation.test.cpp.o.d -o CMakeFiles/simulation.t.dir/simulation.test.cpp.o -c /home/maruan/project_boids/simulation.test.cpp

CMakeFiles/simulation.t.dir/simulation.test.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/simulation.t.dir/simulation.test.cpp.i"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E /home/maruan/project_boids/simulation.test.cpp > CMakeFiles/simulation.t.dir/simulation.test.cpp.i

CMakeFiles/simulation.t.dir/simulation.test.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/simulation.t.dir/simulation.test.cpp.s"
	/usr/bin/c++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S /home/maruan/project_boids/simulation.test.cpp -o CMakeFiles/simulation.t.dir/simulation.test.cpp.s

# Object files for target simulation.t
simulation_t_OBJECTS = \
"CMakeFiles/simulation.t.dir/simulation.test.cpp.o"

# External object files for target simulation.t
simulation_t_EXTERNAL_OBJECTS =

simulation.t: CMakeFiles/simulation.t.dir/simulation.test.cpp.o
simulation.t: CMakeFiles/simulation.t.dir/build.make
simulation.t: /usr/lib/x86_64-linux-gnu/libsfml-graphics.so.2.5.1
simulation.t: /usr/lib/x86_64-linux-gnu/libsfml-window.so.2.5.1
simulation.t: /usr/lib/x86_64-linux-gnu/libsfml-system.so.2.5.1
simulation.t: CMakeFiles/simulation.t.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir=/home/maruan/project_boids/build/CMakeFiles --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable simulation.t"
	$(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/simulation.t.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
CMakeFiles/simulation.t.dir/build: simulation.t
.PHONY : CMakeFiles/simulation.t.dir/build

CMakeFiles/simulation.t.dir/clean:
	$(CMAKE_COMMAND) -P CMakeFiles/simulation.t.dir/cmake_clean.cmake
.PHONY : CMakeFiles/simulation.t.dir/clean

CMakeFiles/simulation.t.dir/depend:
	cd /home/maruan/project_boids/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /home/maruan/project_boids /home/maruan/project_boids /home/maruan/project_boids/build /home/maruan/project_boids/build /home/maruan/project_boids/build/CMakeFiles/simulation.t.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : CMakeFiles/simulation.t.dir/depend

