#!/bin/sh

OPENCV_DIR="thirdparty/opencv"
OPENCV_BUILD_DIR="${OPENCV_DIR}/build_static"

for x in $( grep -nriI "dlopen" "${OPENCV_DIR}" | awk -F: '{print $1}' | sort -u | awk -F/ '{ print $NF}' | sed "s/\.cpp/.o/g" )
do
	echo $x
	find "${OPENCV_BUILD_DIR}" -name "$x"
done
