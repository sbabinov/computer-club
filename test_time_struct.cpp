#include <iostream>
#include <fstream>
#include <limits>
#include "gtest/gtest.h"
#include "structs.hpp"

TEST(TimeTest, MethodsTest)
{
  using structs::Time;
  Time t1({1, 2});
  Time t2({3, 4});

  EXPECT_EQ(t1 < t2, true);
  EXPECT_EQ(t1 > t2, false);
  EXPECT_EQ(t1 <= t2, true);
  EXPECT_EQ(t1 >= t2, false);
  EXPECT_EQ(t2 - t1, Time({2, 2}));
  EXPECT_EQ(Time({23, 6}) - Time({18, 20}), Time({4, 46}));
  EXPECT_EQ(t1 + t2, Time({4, 6}));
}

TEST(TimeTest, InputTest)
{
  using structs::Time;
  std::ifstream file("../inputs/time.txt");
  ASSERT_TRUE(file.is_open()) << "Cannot open file";
  Time t{};

  file >> t;
  EXPECT_EQ(t.hours, 11);
  EXPECT_EQ(t.minutes, 35);
  EXPECT_FALSE(file.fail());

  file >> t;
  while (!file.eof())
  {
    EXPECT_TRUE(file.fail());
    file.clear();
    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    file >> t;
  }

  file.close();
}

TEST(TimeTest, OutputTest)
{
  using structs::Time;

  testing::internal::CaptureStdout();
  std::cout << Time({23, 23});
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "23:23");

  testing::internal::CaptureStdout();
  std::cout << Time({5, 5});
  output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "05:05");

  testing::internal::CaptureStdout();
  std::cout << Time({0, 0});
  output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "00:00");
}
