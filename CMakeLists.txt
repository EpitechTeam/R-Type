cmake_minimum_required(VERSION 3.10.2)
project(R-Type)

set(CMAKE_CXX_STANDARD 14)

include_directories(src/client/source/include)
include_directories(src/client/source/lib)
include_directories(src/server/include)

include_directories(include)
link_directories(lib)

add_executable(R-Type_Client
        src/client/source/include/Client.hpp
        src/client/source/lib/Map.cpp
        src/client/source/lib/Map.hpp
        src/client/source/lib/Menu.cpp
        src/client/source/lib/Menu.hpp
        src/client/source/lib/Connection.cpp
        src/client/source/lib/Connection.hpp
        src/client/source/lib/CreateRoom.cpp
        src/client/source/lib/CreateRoom.hpp
        src/client/source/lib/Room.cpp
        src/client/source/lib/Room.hpp
        src/client/source/lib/JoinRoom.cpp
        src/client/source/lib/JoinRoom.hpp
        src/client/source/lib/Auth.cpp
        src/client/source/lib/Auth.hpp
        src/client/source/lib/Game.cpp
        src/client/source/lib/Game.hpp
        src/client/source/lib/Starship.cpp
        src/client/source/lib/Starship.hpp
        src/client/source/lib/Animation.cpp
        src/client/source/lib/Animation.hpp
        src/client/source/lib/Bullet.cpp
        src/client/source/lib/Bullet.hpp
        src/client/source/lib/Mob.cpp
        src/client/source/lib/Mob.hpp
        src/client/source/lib/utils.cpp
        src/client/source/main.cpp)

target_link_libraries(R-Type_Client sfml-graphics)
target_link_libraries(R-Type_Client sfml-window)
target_link_libraries(R-Type_Client sfml-system )

target_link_libraries (R-Type_Client boost_thread)
target_link_libraries (R-Type_Client pthread)
target_link_libraries (R-Type_Client boost_regex)
target_link_libraries (R-Type_Client boost_system)

add_executable(R-Type_Server
        src/server/src/GameCommands.cpp
        src/server/src/main.cpp
        src/server/src/Parser.cpp
        src/server/src/Participant.cpp
        src/server/src/PreGameCommands.cpp
        src/server/src/Room.cpp
        src/server/src/Server.cpp
        src/server/src/Session.cpp)

target_link_libraries (R-Type_Server boost_thread)
target_link_libraries (R-Type_Server pthread)
target_link_libraries (R-Type_Server boost_regex)
target_link_libraries (R-Type_Server boost_system)

set (CMAKE_CXX_FLAGS "-g3 -Wall -O0")

set(EXECUTABLE_OUTPUT_PATH ./)
set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
