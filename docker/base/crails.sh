#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 1c65020ab72ff085a6e3daf42eed5dd979ae8948

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
