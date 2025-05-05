#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <iterator>

TEST(MainTest, MainTest)
{
  using iter = std::istreambuf_iterator< char >;

  for (size_t i = 1; i <= 7; ++i)
  {
    std::string cmd = "./main ../input/inp" + std::to_string(i) + ".txt > output.txt";
    std::system(cmd.c_str());

    std::ifstream out1("../expected_output/out" + std::to_string(i) + ".txt");
    std::ifstream out2("output.txt");
    EXPECT_TRUE(std::equal(iter(out1), iter(), iter(out2)));
  }
  std::remove("output.txt");
}
