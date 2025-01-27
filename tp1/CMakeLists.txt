cmake_minimum_required(VERSION 3.10)
project(tp1 CXX)

find_package(OpenMP REQUIRED)
if(OpenMP_CXX_FOUND)
    set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} ${OpenMP_CXX_FLAGS}")
endif()

add_executable(tp1 hello_omp.cpp)
target_link_libraries(tp1 OpenMP::OpenMP_CXX)

