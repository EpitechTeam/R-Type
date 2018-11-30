cmake_minimum_required(VERSION 3.10.2)
project(R-Type)

set(CMAKE_CXX_STANDARD 14)

message(STATUS "start running Server cmake...")

set (CMAKE_RUNTIME_OUTPUT_DIRECTORY .)

if (UNIX AND NOT APPLE)
    set(LINUX TRUE)
endif()

if (APPLE)
    message(STATUS "OSX")

#    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)
#    conan_basic_setup()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    file(COPY src/client/asset
            DESTINATION ${CMAKE_BINARY_DIR})

    file(COPY src/client/myasset
            DESTINATION ${CMAKE_BINARY_DIR})

    file(COPY monsters
            DESTINATION ${CMAKE_BINARY_DIR})


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


    include_directories(src/client/source/include)
    include_directories(src/client/source/lib)
    file(GLOB CLIENTSOURCES "src/client/source/lib/*.cpp")

    add_executable(R_Type_Client ${CLIENTSOURCES})

    if(Boost_FOUND)
        target_link_libraries(R_Type ${Boost_LIBRARIES})
    endif()

#    target_link_libraries(R-Type_Client sfml-graphics)
#    target_link_libraries(R-Type_Client sfml-window)
#    target_link_libraries(R-Type_Client sfml-system)
#    target_link_libraries(R-Type_Client sfml-audio)

#    target_link_libraries(R-Type_Client ${CONAN_LIBS})

    set(CMAKE_CXX_FLAGS "-g3 -Wall -O0 -std=c++11")

endif ()

if (LINUX)
    message(STATUS "LINUX")

    include(${CMAKE_BINARY_DIR}/conanbuildinfo.cmake)

    conan_basic_setup()

    set(CMAKE_RUNTIME_OUTPUT_DIRECTORY ${CMAKE_BINARY_DIR})

    file(COPY src/client/asset
            DESTINATION ${CMAKE_BINARY_DIR})

    file(COPY src/client/myasset
            DESTINATION ${CMAKE_BINARY_DIR})

    file(COPY monsters
            DESTINATION ${CMAKE_BINARY_DIR})

    include_directories(src/server/include)

    add_executable(R-Type_Server
            src/server/src/Entities.cpp
            src/server/src/UDPGame.cpp
            src/server/src/main.cpp
            src/server/src/Parser.cpp
            src/server/src/Participant.cpp
            src/server/src/PreGameCommands.cpp
            src/server/src/Room.cpp
            src/server/src/Server.cpp
            src/server/src/Session.cpp
            src/server/src/UDPServer.cpp
            )

    target_link_libraries(R-Type_Server pthread)
    target_link_libraries(R-Type_Server ${CONAN_LIBS})

    include_directories(src/client/source/include)
    include_directories(src/client/source/lib)

    add_executable(R-Type_Client
            src/client/source/include/UDPClient.cpp
            src/client/source/lib/Animation.cpp
            src/client/source/lib/Auth.cpp
            src/client/source/lib/Bullet.cpp
            src/client/source/lib/Connection.cpp
            src/client/source/lib/CreateRoom.cpp
            src/client/source/lib/JoinRoom.cpp
            src/client/source/lib/Map.cpp
            src/client/source/lib/Menu.cpp
            src/client/source/lib/Mob.cpp
            src/client/source/lib/Room.cpp
            src/client/source/lib/RType.cpp
            src/client/source/lib/Starship.cpp
            src/client/source/lib/utils.cpp
            src/client/source/lib/Game.cpp
            src/client/source/main.cpp
            )

    target_link_libraries(R-Type_Client sfml-graphics)
    target_link_libraries(R-Type_Client sfml-window)
    target_link_libraries(R-Type_Client sfml-system)
    target_link_libraries(R-Type_Client sfml-audio)

    target_link_libraries(R-Type_Server pthread)
    target_link_libraries(R-Type_Client ${CONAN_LIBS})

    set(CMAKE_CXX_FLAGS "-g3 -Wall -O0 -std=c++11")

endif ()

if (WIN32)
    message(STATUS "WINDOWS")
endif ()