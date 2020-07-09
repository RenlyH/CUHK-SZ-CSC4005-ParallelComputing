cmake_minimum_required(VERSION 3.15)
project(parallel)

set(INC_DIR /opt/X11/include)
set(LINK_DIR /opt/X11/lib)
include_directories(${INC_DIR})
link_directories(${LINK_DIR})
link_libraries(X11)

find_package(MPI REQUIRED)
include_directories(${MPI_INCLUDE_PATH})
set(CMAKE_C_COMPILER /usr/local/Cellar/mpich/3.3.1/bin/mpicc)
set(CMAKE_CXX_COMPILER /usr/local/Cellar/mpich/3.3.1/bin/mpic++)
set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")

add_executable(parallel main.cpp)

add_subdirectory(HW1)
add_subdirectory(HW2)
add_subdirectory(HW3)
add_subdirectory(HW4)


