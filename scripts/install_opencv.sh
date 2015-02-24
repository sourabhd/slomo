#!/bin/sh

set -vx

SCRIPTS_DIR="scripts"
INSTALL_DIR="thirdparty"
OPENCV_DIR="${INSTALL_DIR}/opencv"
OPENCV_BUILD_DIR="${OPENCV_DIR}/build"
RELEASE="3.0.0"

# Install dependencies

sudo apt-get install build-essential
sudo apt-get install cmake git libgtk2.0-dev pkg-config libavcodec-dev libavformat-dev libswscale-dev
sudo apt-get install python-dev python-numpy libtbb2 libtbb-dev libjpeg-dev libpng-dev libtiff-dev libjasper-dev libdc1394-22-dev


# Clone project

mkdir -p "${INSTALL_DIR}"
cd "${INSTALL_DIR}"
git clone "https://github.com/Itseez/opencv.git"
cd ..
cd "${OPENCV_DIR}"
git checkout $RELEASE
cd ../..


# Build

mkdir -p "${OPENCV_BUILD_DIR}"
cd "${OPENCV_BUILD_DIR}"
cp "${SCRIPTS_DIR}/opencv_gtk_CMakeCache.txt" "${OPENCV_BUILD_DIR}/CMakeCache.txt"
ccmake -G 'Unix Makefiles' -DCMAKE_VERBOSE_MAKEFILE=ON ..
cmake -G 'Unix Makefiles' -DCMAKE_VERBOSE_MAKEFILE=ON ..
make
make install
cd ../..
