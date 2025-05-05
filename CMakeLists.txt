cmake_minimum_required(VERSION 3.14)

project(computer-club)

find_package(GTest REQUIRED)
include(GoogleTest)

add_executable(main main.cpp events.cpp guards.cpp club.cpp structs.cpp)
add_executable(tests test_time_struct.cpp test_events.cpp test_main.cpp events.cpp guards.cpp club.cpp structs.cpp)

target_link_libraries(tests
    GTest::GTest
    GTest::Main
)

gtest_discover_tests(tests)
