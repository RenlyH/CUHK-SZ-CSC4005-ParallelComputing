# CMAKE generated file: DO NOT EDIT!
# Generated by "Unix Makefiles" Generator, CMake Version 3.15

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
CMAKE_COMMAND = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake

# The command to remove a file.
RM = /Applications/CLion.app/Contents/bin/cmake/mac/bin/cmake -E remove -f

# Escaping for special characters.
EQUALS = =

# The top-level source directory on which CMake was run.
CMAKE_SOURCE_DIR = "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel"

# The top-level build directory on which CMake was run.
CMAKE_BINARY_DIR = "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug"

# Include any dependencies generated for this target.
include HW2/CMakeFiles/moving.dir/depend.make

# Include the progress variables for this target.
include HW2/CMakeFiles/moving.dir/progress.make

# Include the compile flags for this target's objects.
include HW2/CMakeFiles/moving.dir/flags.make

HW2/CMakeFiles/moving.dir/x11_moving_object.cpp.o: HW2/CMakeFiles/moving.dir/flags.make
HW2/CMakeFiles/moving.dir/x11_moving_object.cpp.o: ../HW2/x11_moving_object.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object HW2/CMakeFiles/moving.dir/x11_moving_object.cpp.o"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2" && /usr/local/Cellar/mpich/3.3.1/bin/mpic++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/moving.dir/x11_moving_object.cpp.o -c "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW2/x11_moving_object.cpp"

HW2/CMakeFiles/moving.dir/x11_moving_object.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/moving.dir/x11_moving_object.cpp.i"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2" && /usr/local/Cellar/mpich/3.3.1/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW2/x11_moving_object.cpp" > CMakeFiles/moving.dir/x11_moving_object.cpp.i

HW2/CMakeFiles/moving.dir/x11_moving_object.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/moving.dir/x11_moving_object.cpp.s"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2" && /usr/local/Cellar/mpich/3.3.1/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW2/x11_moving_object.cpp" -o CMakeFiles/moving.dir/x11_moving_object.cpp.s

# Object files for target moving
moving_OBJECTS = \
"CMakeFiles/moving.dir/x11_moving_object.cpp.o"

# External object files for target moving
moving_EXTERNAL_OBJECTS =

HW2/moving: HW2/CMakeFiles/moving.dir/x11_moving_object.cpp.o
HW2/moving: HW2/CMakeFiles/moving.dir/build.make
HW2/moving: HW2/CMakeFiles/moving.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable moving"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/moving.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
HW2/CMakeFiles/moving.dir/build: HW2/moving

.PHONY : HW2/CMakeFiles/moving.dir/build

HW2/CMakeFiles/moving.dir/clean:
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2" && $(CMAKE_COMMAND) -P CMakeFiles/moving.dir/cmake_clean.cmake
.PHONY : HW2/CMakeFiles/moving.dir/clean

HW2/CMakeFiles/moving.dir/depend:
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW2" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW2/CMakeFiles/moving.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : HW2/CMakeFiles/moving.dir/depend
