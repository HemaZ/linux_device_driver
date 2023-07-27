#ifndef __LINUX_TEST_DRIVER_HPP__
#define __LINUX_TEST_DRIVER_HPP__
#include <fstream>
#include <iostream>
#include <string>
class Driver
{
private:
    const std::string deviceFilePath = "/dev/test_file";
    std::fstream deviceFile;

public:
    ~Driver()
    {
        if (deviceFile.is_open())
            deviceFile.close();
    }
    bool write(const std::string &message)
    {
        deviceFile.open(deviceFilePath, std::ios::out);
        deviceFile.write(message.c_str(), message.size());
        deviceFile.close();
        return true;
    }
    std::string read()
    {
        std::string output;
        deviceFile.open(deviceFilePath);
        std::getline(deviceFile, output);
        deviceFile.close();
        return output;
    }
    bool turnOn()
    {
        return write("1");
    }
    bool turnOff()
    {
        return write("0");
    }
};

#endif