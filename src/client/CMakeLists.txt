cmake_minimum_required(VERSION 3.10.2)
project(R-Type_Client)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "start running Client cmake...")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY ../../)

if (APPLE)
    message(STATUS "OSX")

    include_directories(include)

    find_package(Boost 1.68.0 COMPONENTS system filesystem REQUIRED)

    if (Boost_FOUND)

        message(STATUS "Boost_INCLUDE_DIRS: ${Boost_INCLUDE_DIRS}")
        message(STATUS "Boost_LIBRARIES: ${Boost_LIBRARIES}")
        message(STATUS "Boost_VERSION: ${Boost_VERSION}")

        include_directories(${Boost_INCLUDE_DIRS})

    endif ()


    file(GLOB SOURCES "source/*.cpp")

    add_executable(R_Type ${SOURCES})

    if (Boost_FOUND)

        target_link_libraries(R_Type ${Boost_LIBRARIES})

    endif ()

endif ()

if (UNIX)
    message(STATUS "LINUX")

    include_directories(source/include)
    include_directories(source/lib)

    include_directories(../../include)
    link_directories(../../lib)

    add_executable(R-Type_Client
            source/lib/Animation.cpp
            source/lib/Auth.cpp
            source/lib/Bullet.cpp
            source/lib/Connection.cpp
            source/lib/CreateRoom.cpp
            source/lib/Game.cpp
            source/lib/JoinRoom.cpp
            source/lib/Map.cpp
            source/lib/Menu.cpp
            source/lib/Mob.cpp
            source/lib/Room.cpp
            source/lib/RType.cpp
            source/lib/Starship.cpp
            source/lib/utils.cpp
            source/main.cpp
            )

    target_link_libraries(R-Type_Client sfml-graphics)
    target_link_libraries(R-Type_Client sfml-window)
    target_link_libraries(R-Type_Client sfml-system)
    target_link_libraries(R-Type_Client sfml-audio)

    target_link_libraries(R-Type_Client boost_thread)
    target_link_libraries(R-Type_Client pthread)
    target_link_libraries(R-Type_Client boost_regex)
    target_link_libraries(R-Type_Client boost_system)

    set(CMAKE_CXX_FLAGS "-g3 -Wall -O0 -std=c++11")

    set(EXECUTABLE_OUTPUT_PATH ./)
    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

endif ()

if (WIN32)
    message(STATUS "WINDOWS")
endif ()