#include <iostream>
#include <fstream>
#include <limits>
#include "gtest/gtest.h"
#include "../src/structs.hpp"
#include "../src/events.hpp"

TEST(EventTest, IOTest)
{
  using namespace events;
  std::ifstream file("../tests/input/events.txt");
  ASSERT_TRUE(file.is_open()) << "Cannot open file";

  auto event = std::make_unique< ClientEvent >();
  file >> event;
  EXPECT_EQ(typeid(*event), typeid(ClientCameEvent));
  testing::internal::CaptureStdout();
  std::cout << *event;
  std::string output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "08:48 1 client1");

  file >> event;
  EXPECT_EQ(typeid(*event), typeid(ClientSatEvent));
  testing::internal::CaptureStdout();
  std::cout << *event;
  output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "09:34 2 client2 1");

  file >> event;
  EXPECT_EQ(typeid(*event), typeid(ClientWaitingEvent));
  testing::internal::CaptureStdout();
  std::cout << *event;
  output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "11:35 3 client3");

  file >> event;
  EXPECT_EQ(typeid(*event), typeid(ClientLeftEvent));
  testing::internal::CaptureStdout();
  std::cout << *event;
  output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "20:25 4 client2");

  testing::internal::CaptureStdout();
  std::cout << ErrorEvent(structs::Time({9, 9}), ErrorType::NOT_OPEN_YET);
  output = testing::internal::GetCapturedStdout();
  EXPECT_EQ(output, "09:09 13 NotOpenYet");

  EXPECT_FALSE(file.fail());

  file >> event;
  while (!file.eof())
  {
    EXPECT_TRUE(file.fail());
    file.clear();
    file.ignore(std::numeric_limits< std::streamsize >::max(), '\n');
    file >> event;
  }
  file.close();
}
