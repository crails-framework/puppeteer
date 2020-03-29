#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 83f61e812786da21b8ae794d3679f33517a21d37

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
