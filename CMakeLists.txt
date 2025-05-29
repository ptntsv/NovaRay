set(CMAKE_CXX_COMPILER /usr/bin/clang++)
set(CMAKE_CXX_FLAGS -fsanitize=address)
# set(CMAKE_EXE_LINKER_FLAGS -pg)
cmake_minimum_required(VERSION 3.29)
project(NovaRay)

set(CMAKE_CXX_STANDARD 20)
set(CMAKE_CXX_STANDARD_REQUIRED ON)

enable_testing()

add_executable(main main.cpp)
target_compile_options(main PRIVATE "${CMAKE_CXX_FLAGS}")

add_executable(main_pg main.cpp)
target_compile_options(main_pg PRIVATE -pg)
target_link_options(main_pg PRIVATE -pg)

add_subdirectory(src)
add_subdirectory(tests)
