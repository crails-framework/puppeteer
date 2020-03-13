#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout a0a6d9ef24d22d0185334dba8c363874fedca044

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
