#!/bin/sh
set -vx
BASE_DIR="/home/sourabhd/projects/slomo"
OPENCV_DIR="${BASE_DIR}/thirdparty/opencv"
BUILD_DIR="${OPENCV_DIR}/build_static"
BIN_DIR="${BASE_DIR}/bin"
SRC_DIR="${BASE_DIR}/src"
TARGET="slomo"
SOURCES="delaunay2.cpp SloMo.cpp main.cpp"
OBJECTS="delaunay2.o SloMo.o main.o"
export PKG_CONFIG_PATH="${OPENCV_DIR}/unix-install:${PKG_CONFIG_PATH}"
cd "${BASE_DIR}"
cd "${SRC_DIR}"

# -I/home/sourabhd/projects/slomo/build_static/include/opencv
# -I/home/sourabhd/projects/slomo/build_static/include
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_calib3d.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_core.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_features2d.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_flann.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_highgui.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_imgcodecs.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_imgproc.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_ml.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_objdetect.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_photo.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_shape.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_stitching.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_superres.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_ts.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_video.a
# /home/sourabhd/projects/slomo/build_static/lib/libopencv_videoio.a



g++ -c -Wall -std=c++11 -fopenmp -static \
 -I/home/sourabhd/projects/slomo/build_static/include/opencv \
 -I/home/sourabhd/projects/slomo/build_static/include \
 $SOURCES

#g++ -v -Wall -std=c++11 -fopenmp -static -static-libstdc++ \
g++ -v -static \
 -I/home/sourabhd/projects/slomo/build_static/include/opencv \
 -I/home/sourabhd/projects/slomo/build_static/include \
 -o ${BIN_DIR}/${TARGET} \
 -Wl,--start-group \
 /usr/lib/x86_64-linux-gnu/libm.a \
 /usr/lib/gcc/x86_64-linux-gnu/4.8.2/libgcc.a \
 /usr/lib/gcc/x86_64-linux-gnu/4.8.2/libgcc_eh.a \
 /usr/lib/gcc/x86_64-linux-gnu/4.8.2/libstdc++.a \
 /usr/lib/gcc/x86_64-linux-gnu/4.8.2/libgomp.a \
 /usr/lib/x86_64-linux-gnu/libdl.a \
 /usr/lib/x86_64-linux-gnu/libz.a \
 /usr/lib/x86_64-linux-gnu/liblzma.a \
 /usr/lib/x86_64-linux-gnu/libxml2.a \
 /usr/lib/x86_64-linux-gnu/libgobject-2.0.a \
 /usr/lib/x86_64-linux-gnu/libgmodule-2.0.a \
 /usr/lib/x86_64-linux-gnu/libglib-2.0.a \
 /usr/lib/x86_64-linux-gnu/libpcre.a \
 /usr/lib/x86_64-linux-gnu/libspeex.a \
 /usr/lib/x86_64-linux-gnu/liborc-0.4.a \
 /usr/lib/x86_64-linux-gnu/libschroedinger-1.0.a \
 /usr/lib/x86_64-linux-gnu/libmp3lame.a \
 /usr/lib/x86_64-linux-gnu/libxvidcore.a \
 /home/sourabhd/projects/slomo/thirdparty/gstreamer-0.10.36_install/lib/libgstbase-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gstreamer-0.10.36_install/lib/libgstcheck-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gstreamer-0.10.36_install/lib/libgstcontroller-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gstreamer-0.10.36_install/lib/libgstdataprotocol-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gstreamer-0.10.36_install/lib/libgstnet-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gstreamer-0.10.36_install/lib/libgstreamer-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstapp-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstaudio-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstcdda-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstfft-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstinterfaces-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstnetbuffer-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstpbutils-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstriff-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstrtp-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstrtsp-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstsdp-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgsttag-0.10.a \
 /home/sourabhd/projects/slomo/thirdparty/gst-plugins-base-0.10.36_install/lib/libgstvideo-0.10.a \
 /usr/lib/x86_64-linux-gnu/librtmp.a \
 /usr/lib/x86_64-linux-gnu/libz.a \
 /usr/lib/x86_64-linux-gnu/libm.a \
 /usr/lib/x86_64-linux-gnu/libbz2.a \
 /usr/lib/x86_64-linux-gnu/libpthread.a \
 /usr/lib/x86_64-linux-gnu/libxvidcore.a \
 /usr/lib/x86_64-linux-gnu/libx264.a \
 /usr/lib/x86_64-linux-gnu/libvpx.a \
 /usr/lib/x86_64-linux-gnu/libvorbisenc.a \
 /usr/lib/x86_64-linux-gnu/libvorbis.a \
 /usr/lib/x86_64-linux-gnu/libtheoraenc.a \
 /usr/lib/x86_64-linux-gnu/libtheoradec.a \
 /usr/lib/x86_64-linux-gnu/libspeex.a \
 /usr/lib/x86_64-linux-gnu/libschroedinger-1.0.a \
 /usr/lib/x86_64-linux-gnu/libopus.a \
 /usr/lib/x86_64-linux-gnu/libopenjpeg.a \
 /usr/lib/x86_64-linux-gnu/libmp3lame.a \
 /usr/lib/x86_64-linux-gnu/libgsm.a \
 /usr/lib/x86_64-linux-gnu/libgcrypt.a \
 /usr/lib/x86_64-linux-gnu/libtasn1.a \
 /usr/lib/x86_64-linux-gnu/libogg.a \
 /usr/lib/x86_64-linux-gnu/liborc-0.4.a \
 /usr/lib/x86_64-linux-gnu/libgpg-error.a \
 /usr/lib/x86_64-linux-gnu/libffi.a \
 /home/sourabhd/projects/slomo/thirdparty/libav-9.16_install/lib/libavutil.a \
 /home/sourabhd/projects/slomo/thirdparty/libav-9.16_install/lib/libavcodec.a \
 /home/sourabhd/projects/slomo/thirdparty/libav-9.16_install/lib/libavformat.a \
 /home/sourabhd/projects/slomo/thirdparty/libav-9.16_install/lib/libavresample.a \
 /home/sourabhd/projects/slomo/thirdparty/libav-9.16_install/lib/libswscale.a \
 /home/sourabhd/projects/slomo/thirdparty/opencv/build_static/3rdparty/lib/liblibpng.a \
 /usr/lib/x86_64-linux-gnu/libjpeg.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_core.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_features2d.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_flann.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_highgui.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_imgcodecs.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_imgproc.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_ml.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_objdetect.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_photo.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_shape.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_stitching.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_superres.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_ts.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_video.a \
 /home/sourabhd/projects/slomo/build_static/lib/libopencv_videoio.a \
 delaunay2.o SloMo.o main.o \
 -Wl,--end-group

 # /usr/lib/x86_64-linux-gnu/linux-vdso.so.1
 # /usr/lib/x86_64-linux-gnu/libva
 # /usr/lib/x86_64-linux-gnu/libfdk-aac
 # /usr/lib/x86_64-linux-gnu/libp11-kit.so
 # /lib64/ld-linux-x86-64.so.2
 # /usr/lib/x86_64-linux-gnu/libgnutls.a \
 # /usr/lib/x86_64-linux-gnu/libpthread.a \
 # /usr/lib/x86_64-linux-gnu/libc.a \

cd ${BASE_DIR}
