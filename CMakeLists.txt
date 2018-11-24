cmake_minimum_required(VERSION 3.10.2)
project(R-Type)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "start running cmake...")

if (APPLE)
    message(STATUS "OSX")

    include_directories(src/server/include)

    find_package(Boost 1.68.0 COMPONENTS system filesystem REQUIRED)

    if(Boost_FOUND)

        message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
        message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
        message(STATUS "Boost_VERSION: ${Boost_VERSION}")

        include_directories(${Boost_INCLUDE_DIRS})

    endif()


    file(GLOB SOURCES "src/server/src/*.cpp")

    add_executable(R_Type ${SOURCES})

    if(Boost_FOUND)

        target_link_libraries(R_Type ${Boost_LIBRARIES})

    endif()

endif()

if (LINUX)
    message(STATUS "LINUX")

    include_directories(src/client/source/include)
    include_directories(src/client/source/lib)
    include_directories(src/server/include)

    include_directories(include)
    link_directories(lib)

    add_executable(R-Type_Client
            src/client/source/lib/utils.cpp
            src/client/source/lib/Map.cpp
            src/client/source/lib/Menu.cpp
            src/client/source/lib/Connection.cpp
            src/client/source/lib/CreateRoom.cpp
            src/client/source/lib/Room.cpp
            src/client/source/lib/JoinRoom.cpp
            src/client/source/lib/Auth.cpp
            src/client/source/lib/Game.cpp
            src/client/source/lib/Starship.cpp
            src/client/source/lib/Animation.cpp
            src/client/source/lib/Bullet.cpp
            src/client/source/lib/Mob.cpp
            src/client/source/main.cpp)

    target_link_libraries(R-Type_Client sfml-graphics)
    target_link_libraries(R-Type_Client sfml-window)
    target_link_libraries(R-Type_Client sfml-system)
    target_link_libraries(R-Type_Client sfml-audio)

    target_link_libraries (R-Type_Client boost_thread)
    target_link_libraries (R-Type_Client pthread)
    target_link_libraries (R-Type_Client boost_regex)
    target_link_libraries (R-Type_Client boost_system)

    add_executable(R-Type_Server
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

    set (CMAKE_CXX_FLAGS "-g3 -Wall -O0 -std=c++11")

    set(EXECUTABLE_OUTPUT_PATH ./)
    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})
endif()

if (WIN32)
    message(STATUS "WINDOWS")
endif()
