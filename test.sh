#!/bin/bash
cd driver
make clean
sudo dmesg -c
make
sudo rmmod hello
sudo insmod hello.ko
sudo chmod 777 /dev/hello_dev
sudo dmesg

cd ../cpp_library
cmake -S . -B build
cd build
make
ctest
