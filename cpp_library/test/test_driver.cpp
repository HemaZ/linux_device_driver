#include <gtest/gtest.h>
#include "driver.hpp"
// Demonstrate some basic assertions.
TEST(HelloTest, BasicReadWrite)
{
    Driver driver;
    std::string message = "Hello World !!!!";
    driver.write(message);
    // Expect two strings not to be equal.
    auto readRes = driver.read();
    EXPECT_EQ(message, readRes);
}