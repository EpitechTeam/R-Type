cmake_minimum_required(VERSION 3.12)
project(R_Type)

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
endif()

if (WIN32)
    message(STATUS "WINDOWS")
endif()
