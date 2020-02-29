#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout efbb24769f131e438ff0a5a2373dfbb1ac1866ac 

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
