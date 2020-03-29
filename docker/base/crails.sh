#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout e406f955f7eb77ea9a46d84062fdd01ff2fe62d5

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
