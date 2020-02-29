#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 2dd7acb41a3d47e6b9e660a469f13fc7b8b3bb07

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
