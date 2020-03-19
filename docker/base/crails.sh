#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout ec7b29cb0018f2621e80bb1b24685f21692d8af2

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
