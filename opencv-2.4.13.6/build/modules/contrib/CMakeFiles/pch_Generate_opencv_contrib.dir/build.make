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

# Utility rule file for pch_Generate_opencv_contrib.

# Include the progress variables for this target.
include modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/progress.make

modules/contrib/CMakeFiles/pch_Generate_opencv_contrib: modules/contrib/precomp.hpp.gch/opencv_contrib_Release.gch

modules/contrib/precomp.hpp.gch/opencv_contrib_Release.gch: ../modules/contrib/src/precomp.hpp
modules/contrib/precomp.hpp.gch/opencv_contrib_Release.gch: modules/contrib/precomp.hpp
modules/contrib/precomp.hpp.gch/opencv_contrib_Release.gch: lib/libopencv_contrib_pch_dephelp.a
	$(CMAKE_COMMAND) -E cmake_progress_report /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/CMakeFiles $(CMAKE_PROGRESS_1)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp.gch/opencv_contrib_Release.gch"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib && /usr/bin/cmake -E make_directory /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib/precomp.hpp.gch
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib && /usr/bin/c++ -O3 -DNDEBUG -DNDEBUG -fPIC "-D_USE_MATH_DEFINES" "-D__STDC_CONSTANT_MACROS" "-D__STDC_LIMIT_MACROS" "-D__STDC_FORMAT_MACROS" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/nonfree/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/ocl/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/gpu/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/photo/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/objdetect/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/legacy/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/video/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/ml/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/calib3d/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/features2d/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/highgui/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/imgproc/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/flann/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/core/include" -isystem"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/contrib/src" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/contrib/include" -isystem"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/nonfree/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/ocl/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/gpu/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/photo/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/objdetect/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/legacy/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/video/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/ml/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/calib3d/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/features2d/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/highgui/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/imgproc/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/flann/include" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/core/include" -isystem"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/contrib/src" -I"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/contrib/include" -isystem"/users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build" -fsigned-char -W -Wall -Werror=return-type -Werror=address -Werror=sequence-point -Wformat -Werror=format-security -Wmissing-declarations -Wundef -Winit-self -Wpointer-arith -Wshadow -Wsign-promo -Wno-narrowing -Wno-delete-non-virtual-dtor -Wno-comment -Wno-array-bounds -Wno-aggressive-loop-optimizations -fdiagnostics-show-option -Wno-long-long -pthread -fomit-frame-pointer -msse -msse2 -msse3 -ffunction-sections -DCVAPI_EXPORTS -x c++-header -o /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib/precomp.hpp.gch/opencv_contrib_Release.gch /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib/precomp.hpp

modules/contrib/precomp.hpp: ../modules/contrib/src/precomp.hpp
	$(CMAKE_COMMAND) -E cmake_progress_report /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/CMakeFiles $(CMAKE_PROGRESS_2)
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --blue --bold "Generating precomp.hpp"
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib && /usr/bin/cmake -E copy_if_different /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/contrib/src/precomp.hpp /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib/precomp.hpp

pch_Generate_opencv_contrib: modules/contrib/CMakeFiles/pch_Generate_opencv_contrib
pch_Generate_opencv_contrib: modules/contrib/precomp.hpp.gch/opencv_contrib_Release.gch
pch_Generate_opencv_contrib: modules/contrib/precomp.hpp
pch_Generate_opencv_contrib: modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/build.make
.PHONY : pch_Generate_opencv_contrib

# Rule to build all files generated by this target.
modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/build: pch_Generate_opencv_contrib
.PHONY : modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/build

modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/clean:
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib && $(CMAKE_COMMAND) -P CMakeFiles/pch_Generate_opencv_contrib.dir/cmake_clean.cmake
.PHONY : modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/clean

modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/depend:
	cd /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6 /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/modules/contrib /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib /users/guest438/scratch/ENGN2560/ENGN2560/opencv-2.4.13.6/build/modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/DependInfo.cmake --color=$(COLOR)
.PHONY : modules/contrib/CMakeFiles/pch_Generate_opencv_contrib.dir/depend

