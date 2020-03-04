#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 35099aac1498ef853cb02e08b6e1842e285a59a9

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
