#include "driver/driver.hpp"
#include <chrono>
#include <thread>
#include <stdio.h>
int main(int argc, char const *argv[])
{
    Driver driver;
    while (true)
    {
        printf("Turning on \n");
        driver.turnOn();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
        printf("Turning Off \n");
        driver.turnOff();
        std::this_thread::sleep_for(std::chrono::milliseconds(500));
    }
    return 0;
}
