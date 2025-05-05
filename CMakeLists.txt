cmake_minimum_required(VERSION 3.14)
project(ComputerClub)

option(COMPUTER_CLUB_BUILD_TESTS "Build tests" ON)

add_executable(main src/main.cpp src/events.cpp src/guards.cpp src/club.cpp src/structs.cpp)

if(COMPUTER_CLUB_BUILD_TESTS)
    find_package(GTest REQUIRED)
    include(GoogleTest)
    add_subdirectory(tests)
endif()
