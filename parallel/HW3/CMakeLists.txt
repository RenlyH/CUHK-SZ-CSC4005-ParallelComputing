#set(CMAKE_CXX_COMPILER mpicxx)
#set(CMAKE_C_COMPILER mpicc)

add_executable(seq seq_N_body.cpp)
add_executable(pth pth_N_body.cpp)
add_executable(mpi mpi_N_body.cpp )

#set(CMAKE_CXX_COMPILER /usr/local/bin/g++-9)
#set(CMAKE_C_COMPILER /usr/local/bin/gcc-9)
### enable openmp using -fopenmp
#set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -std=c++11 -fopenmp")
#

add_executable(omp omp_N_body.cpp)
add_executable(list make_list.cpp)
add_executable(ompi ompi_N_body.cpp)