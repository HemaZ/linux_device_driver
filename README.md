# linux_device_driver

Linux driver implementation based on the great tutorials by [Moatasem Elsayed](https://www.youtube.com/playlist?list=PLkH1REggdbJq8KQBK82xW0K709tye2wTN
). This driver is built and tested on RPI4. it provides a driver file which you can write to it (0,1) to turn on / off a LEd connected to the GPIO(2) pin on the RPI.

## Build 

You can build and run tests using the provided `test.sh` file

`sh test.sh`

or if you wish to build it manually. check below

### Driver

```sh
cd driver && make #Build the driver
sudo insmod hello.ko #Load it in the kernel
sudo chmod 777 /dev/hello_dev #change the driver file permissions
```

### C++ Library

```sh
cd cpp_library
mkdir build && cd build
cmake ..
make
ctest # (Optional) run tests
```

## How to use 

form the terminal you can write directly to the file `/dev/hello_dev`

```sh
echo 1 > /dev/hello_dev # turn on the led
echo 0 > /dev/hello_dev # turn off the led
```

or using the c++ library

```c++
#include "driver/driver.hpp"
#include <stdio.h>
int main(int argc, char const *argv[])
{
    Driver driver;
    printf("Turning on \n");
    driver.turnOn();
    printf("Turning Off \n");
    driver.turnOff();
    return 0;
}

```