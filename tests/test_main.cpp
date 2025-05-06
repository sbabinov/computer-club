#include <gtest/gtest.h>
#include <algorithm>
#include <fstream>
#include <cstdlib>
#include <iterator>

class MainParameterizedTest: public testing::TestWithParam< size_t >
{
protected:
  void SetUp() override
  {
    testNum = GetParam();
    inputFile = "../tests/input/inp" + std::to_string(testNum) + ".txt";
    expectedOutputFile = "../tests/expected_output/out" + std::to_string(testNum) + ".txt";
    actualOutputFile = "output.txt";
  }

  void TearDown() override
  {
    std::remove(actualOutputFile.c_str());
  }

  size_t testNum;
  std::string inputFile;
  std::string expectedOutputFile;
  std::string actualOutputFile;
};

TEST_P(MainParameterizedTest, ProducesCorrectOutput)
{
  using iter = std::istreambuf_iterator< char >;
  std::string cmd = "./main " + inputFile + " > " + actualOutputFile;
  std::system(cmd.c_str());

  std::ifstream expectedOut(expectedOutputFile);
  std::ifstream actualOut(actualOutputFile);

  EXPECT_TRUE(std::equal(iter(expectedOut), iter(), iter(actualOut)));
}

INSTANTIATE_TEST_SUITE_P(
  MainTests,
  MainParameterizedTest,
  testing::Range< size_t >(1, 8)
);
