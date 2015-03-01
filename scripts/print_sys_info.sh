#!/bin/sh

set -vx 
uname -a
cat /proc/cpuinfo
cat /proc/meminfo
gcc --version
g++ --version
ld --version
ar --version
g++ -print-file-name=libc.a
g++ -print-file-name=libstdc++.a
g++ -print-file-name=libc.so
g++ -print-file-name=libstdc++.so
