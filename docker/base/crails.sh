#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 3aaa0c4694e2f06a15a2f2c0ca95f40140f4fd85

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
