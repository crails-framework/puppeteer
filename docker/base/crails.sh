#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 186c01cac82d8aaeb2fb6eca3c1c2f3129f5ce96

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
