#!/bin/sh

set -vx

mkdir -p build
cp scripts/slomo_CMakeCache.txt build/CMakeCache.txt
cd build
ccmake -G 'Unix Makefiles' -DCMAKE_VERBOSE_MAKEFILE=ON ..
cmake -G 'Unix Makefiles' -DCMAKE_VERBOSE_MAKEFILE=ON ..
make
make install
