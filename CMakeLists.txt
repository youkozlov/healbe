cmake_minimum_required(VERSION 3.14)

project(aaa)

set(CMAKE_C_STANDARD 11)
set(CMAKE_C_STANDARD_REQUIRED on)
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -s -march=native -mtune=native -DNDEBUG -O0 -Wall -Wextra -Werror")

set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR}/bin)

add_subdirectory(src)