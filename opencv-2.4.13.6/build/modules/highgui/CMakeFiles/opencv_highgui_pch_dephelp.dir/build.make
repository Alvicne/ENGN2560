# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 2.8

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

# The program to use to edit the cache.
CMAKE_EDIT_COMMAND = /usr/bin/ccmake

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build

# Include any dependencies generated for this target.
include modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/depend.make

# Include the progress variables for this target.
include modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/progress.make

# Include the compile flags for this target's objects.
include modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/flags.make

modules/highgui/opencv_highgui_pch_dephelp.cxx: ../modules/highgui/src/precomp.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating opencv_highgui_pch_dephelp.cxx"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/cmake -E echo \#include\ \"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/highgui/src/precomp.hpp\" > /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/cmake -E echo int\ testfunction\(\)\; >> /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/cmake -E echo int\ testfunction\(\) >> /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/cmake -E echo { >> /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/cmake -E echo \ \ \ \ \return\ 0\; >> /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/cmake -E echo } >> /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/flags.make
modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o: modules/highgui/opencv_highgui_pch_dephelp.cxx
	$(CMAKE_COMMAND) -E cmake_progress_report /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Building CXX object modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/c++   $(CXX_DEFINES) $(CXX_FLAGS) -o CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o -c /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.i"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -E /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx > CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.i

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.s"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && /usr/bin/c++  $(CXX_DEFINES) $(CXX_FLAGS) -S /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/opencv_highgui_pch_dephelp.cxx -o CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.s

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.requires:
.PHONY : modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.requires

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.provides: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.requires
	$(MAKE) -f modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/build.make modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.provides.build
.PHONY : modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.provides

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.provides.build: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o

# Object files for target opencv_highgui_pch_dephelp
opencv_highgui_pch_dephelp_OBJECTS = \
"CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o"

# External object files for target opencv_highgui_pch_dephelp
opencv_highgui_pch_dephelp_EXTERNAL_OBJECTS =

lib/libopencv_highgui_pch_dephelp.a: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o
lib/libopencv_highgui_pch_dephelp.a: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/build.make
lib/libopencv_highgui_pch_dephelp.a: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --red --bold "Linking CXX static library ../../lib/libopencv_highgui_pch_dephelp.a"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && $(CMAKE_COMMAND) -P CMakeFiles/opencv_highgui_pch_dephelp.dir/cmake_clean_target.cmake
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/opencv_highgui_pch_dephelp.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/build: lib/libopencv_highgui_pch_dephelp.a
.PHONY : modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/build

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/requires: modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/opencv_highgui_pch_dephelp.cxx.o.requires
.PHONY : modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/requires

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/clean:
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui && $(CMAKE_COMMAND) -P CMakeFiles/opencv_highgui_pch_dephelp.dir/cmake_clean.cmake
.PHONY : modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/clean

modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/depend: modules/highgui/opencv_highgui_pch_dephelp.cxx
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6 /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/highgui /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/highgui/CMakeFiles/opencv_highgui_pch_dephelp.dir/depend

