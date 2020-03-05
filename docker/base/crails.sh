#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 0603b32b0c4a963da08aaa9dfc0ef086ac0573a1

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
