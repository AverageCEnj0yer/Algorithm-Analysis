cmake_minimum_required(VERSION 3.10)
project(FibonacciProject)

set(CMAKE_CXX_STANDARD 17)

find_package(PkgConfig REQUIRED)
pkg_check_modules(GMP REQUIRED gmp gmpxx)

add_executable(test test.cpp)

target_include_directories(test PRIVATE ${GMP_INCLUDE_DIRS})
target_link_libraries(test PRIVATE ${GMP_LIBRARIES})
