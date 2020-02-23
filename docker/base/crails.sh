#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 7da5224e9255c6d9599c2c5be12f1cd2d137db1a

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
