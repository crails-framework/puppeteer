#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 2d88535607284d53e40b3b4eafa1cd4325f42cb3

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
