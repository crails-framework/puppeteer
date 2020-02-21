#!/bin/sh

# cppnetlib-0.13.0-final.tar.gz won't compile with the latest version of boost
#cppnetlib_version="0.13.0"
#cppnetlib_release="final"
#cppnetlib_filename="cpp-netlib-${cppnetlib_version}-${cppnetlib_release}.tar.gz"

#wget http://downloads.cpp-netlib.org/${cppnetlib_version}/${cppnetlib_filename}
#tar -zxvf ${cppnetlib_filename} && rm ${cppnetlib_filename}

#cd cpp-netlib-${cppnetlib_version}-${cppnetlib_release}
#cmake . && make && make install

##
## Installing from git
##
git clone https://github.com/cpp-netlib/cpp-netlib.git

mkdir cpp-netlib/build
cd cpp-netlib/build

git checkout 0.13-release

git submodule init
git submodule update

cmake -DCMAKE_CXX_FLAGS=-std=c++11 \
  -DCPP-NETLIB_BUILD_TESTS=OFF \
  -DCPP-NETLIB_BUILD_EXAMPLES=OFF \
  -DCPP-NETLIB_BUILD_SHARED_LIBS=ON \
  -DUri_BUILD_TESTS=OFF \
  -DUri_WARNINGS_AS_ERRORS=OFF \
  ..

make
make install
