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
include HW4/CMakeFiles/list4.dir/depend.make

# Include the progress variables for this target.
include HW4/CMakeFiles/list4.dir/progress.make

# Include the compile flags for this target's objects.
include HW4/CMakeFiles/list4.dir/flags.make

HW4/CMakeFiles/list4.dir/make_list.cpp.o: HW4/CMakeFiles/list4.dir/flags.make
HW4/CMakeFiles/list4.dir/make_list.cpp.o: ../HW4/make_list.cpp
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --progress-dir="/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_1) "Building CXX object HW4/CMakeFiles/list4.dir/make_list.cpp.o"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4" && /usr/local/Cellar/mpich/3.3.1/bin/mpic++  $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -o CMakeFiles/list4.dir/make_list.cpp.o -c "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW4/make_list.cpp"

HW4/CMakeFiles/list4.dir/make_list.cpp.i: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Preprocessing CXX source to CMakeFiles/list4.dir/make_list.cpp.i"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4" && /usr/local/Cellar/mpich/3.3.1/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -E "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW4/make_list.cpp" > CMakeFiles/list4.dir/make_list.cpp.i

HW4/CMakeFiles/list4.dir/make_list.cpp.s: cmake_force
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green "Compiling CXX source to assembly CMakeFiles/list4.dir/make_list.cpp.s"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4" && /usr/local/Cellar/mpich/3.3.1/bin/mpic++ $(CXX_DEFINES) $(CXX_INCLUDES) $(CXX_FLAGS) -S "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW4/make_list.cpp" -o CMakeFiles/list4.dir/make_list.cpp.s

# Object files for target list4
list4_OBJECTS = \
"CMakeFiles/list4.dir/make_list.cpp.o"

# External object files for target list4
list4_EXTERNAL_OBJECTS =

HW4/list4: HW4/CMakeFiles/list4.dir/make_list.cpp.o
HW4/list4: HW4/CMakeFiles/list4.dir/build.make
HW4/list4: HW4/CMakeFiles/list4.dir/link.txt
	@$(CMAKE_COMMAND) -E cmake_echo_color --switch=$(COLOR) --green --bold --progress-dir="/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/CMakeFiles" --progress-num=$(CMAKE_PROGRESS_2) "Linking CXX executable list4"
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4" && $(CMAKE_COMMAND) -E cmake_link_script CMakeFiles/list4.dir/link.txt --verbose=$(VERBOSE)

# Rule to build all files generated by this target.
HW4/CMakeFiles/list4.dir/build: HW4/list4

.PHONY : HW4/CMakeFiles/list4.dir/build

HW4/CMakeFiles/list4.dir/clean:
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4" && $(CMAKE_COMMAND) -P CMakeFiles/list4.dir/cmake_clean.cmake
.PHONY : HW4/CMakeFiles/list4.dir/clean

HW4/CMakeFiles/list4.dir/depend:
	cd "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug" && $(CMAKE_COMMAND) -E cmake_depends "Unix Makefiles" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/HW4" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4" "/Users/renly/OneDrive - CUHK-Shenzhen/CUHKSZ/2019-2020 T1/CSC4005/parallel/cmake-build-debug/HW4/CMakeFiles/list4.dir/DependInfo.cmake" --color=$(COLOR)
.PHONY : HW4/CMakeFiles/list4.dir/depend

