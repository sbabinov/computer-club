add_executable(tests
    test_time_struct.cpp
    test_events.cpp
    test_main.cpp
    ../src/structs.cpp
    ../src/guards.cpp
    ../src/events.cpp
    ../src/club.cpp
)

target_link_libraries(tests
  GTest::GTest
  GTest::Main
)

gtest_discover_tests(tests)
