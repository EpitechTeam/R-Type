cmake_minimum_required(VERSION 3.10.2)
project(R_Type)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/client/source/includes)
include_directories(src/client/source/lib)
include_directories(src/server/includes)


add_executable(R_Type
        src/client/source/includes/client.hpp
        src/client/source/lib/Map.cpp
        src/client/source/lib/Map.hpp
        src/client/source/lib/Menu.cpp
        src/client/source/lib/Menu.hpp
        src/client/source/lib/Connection.cpp
        src/client/source/lib/Connection.hpp
        src/client/source/lib/utils.cpp
        src/client/source/main.cpp)


target_link_libraries(R_Type sfml-graphics)
target_link_libraries(R_Type sfml-window)
target_link_libraries(R_Type sfml-system )
set (CMAKE_CXX_FLAGS "-g3 -Wall -O0")

set(EXECUTABLE_OUTPUT_PATH ./)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
