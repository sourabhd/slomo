#!/bin/sh

INSTALL_DIR="thirdparty"
OPENCV_DIR="${INSTALL_DIR}/opencv"
RELEASE="2.4.9.1"

mkdir -p "${INSTALL_DIR}"
#cd "${INSTALL_DIR}"
#git clone "https://github.com/Itseez/opencv.git"
cd "${OPENCV_DIR}"
git checkout $RELEASE

