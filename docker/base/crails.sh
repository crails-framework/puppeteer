#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 510fe81b85fc66b19c7ab2c5c2095f72dfed7968

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
