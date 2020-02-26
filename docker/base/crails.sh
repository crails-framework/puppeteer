#!/bin/sh

git clone https://github.com/Plaristote/crails.git
cd crails

git checkout 2fe1cfed8f619e4da14ce7fdf23999af44a1da47

mkdir build
cd build

cmake .. -DDEVELOPER_MODE=OFF
make clean && make && make install
rm CMakeCache.txt

cmake .. -DDEVELOPER_MODE=ON
make clean && make && make install
rm CMakeCache.txt
