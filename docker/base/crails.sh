#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 4aa739a2732f5d75b8dcbecfedfafeff303d61e5

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
