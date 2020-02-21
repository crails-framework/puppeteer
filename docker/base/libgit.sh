#!/bin/sh

build_dir="libgit2/build"

git clone https://github.com/libgit2/libgit2.git
mkdir -p "$build_dir"
cd "$build_dir"

cmake ..
make
make install
