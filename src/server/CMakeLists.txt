cmake_minimum_required(VERSION 3.10.2)
project(R-Type_Server)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "start running Server cmake...")

if (APPLE)
    message(STATUS "OSX")

    include_directories(src/server/include)

    find_package(Boost 1.68.0 COMPONENTS system filesystem REQUIRED)

    if (Boost_FOUND)

        message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
        message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
        message(STATUS "Boost_VERSION: ${Boost_VERSION}")

        include_directories(${Boost_INCLUDE_DIRS})

    endif ()


    file(GLOB SOURCES "src/server/src/*.cpp")

    add_executable(R-Type_Server ${SOURCES})

    if (Boost_FOUND)

        target_link_libraries(R-Type_Server ${Boost_LIBRARIES})

    endif ()

endif ()

if (UNIX)
    message(STATUS "LINUX")

    include_directories(include)

    include_directories(../../include)
    link_directories(../../lib)

    set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)

    add_executable(R-Type_Server
            src/Entities.cpp
            src/Game.cpp
            src/main.cpp
            src/Parser.cpp
            src/Participant.cpp
            src/PreGameCommands.cpp
            src/Room.cpp
            src/Server.cpp
            src/Session.cpp
            src/UDPServer.cpp
            )

    target_link_libraries(R-Type_Server boost_thread)
    target_link_libraries(R-Type_Server pthread)
    target_link_libraries(R-Type_Server boost_regex)
    target_link_libraries(R-Type_Server boost_system)

    set(CMAKE_CXX_FLAGS "-g3 -Wall -O0 -std=c++11")

    set(EXECUTABLE_OUTPUT_PATH ./)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

endif ()

if (WIN32)
    message(STATUS "WINDOWS")
endif ()