#!/bin/sh

set -vx

BASE_DIR="/home/sourabhd/projects/slomo"
INSTALL_DIR="thirdparty"

#GSTREAMER_0_10_RELEASE="0.10.36"
#GSTREAMER_0_10_URL="http://gstreamer.freedesktop.org/src/gstreamer/gstreamer-${GSTREAMER_0_10_RELEASE}.tar.bz2"
#GSTREAMER_0_10_SRC="gstreamer-${GSTREAMER_0_10_RELEASE}"
#GSTREAMER_0_10_INSTALL_PREFIX="${BASE_DIR}/${INSTALL_DIR}/${GSTREAMER_0_10_SRC}_install"
#GSTREAMER_0_10_SRC_DIR="${INSTALL_DIR}/${GSTREAMER_0_10_SRC}"
#
#cd "${BASE_DIR}"
#mkdir -p "${INSTALL_DIR}"
#mkdir -p "${GSTREAMER_0_10_INSTALL_PREFIX}"
#
#cd "${INSTALL_DIR}"
#wget "${GSTREAMER_0_10_URL}"
#tar xvjf "${GSTREAMER_0_10_SRC}.tar.bz2"
#cd "${BASE_DIR}"
#
#cd "${GSTREAMER_0_10_SRC_DIR}"
## fix error based on http://www.linuxfromscratch.org/pipermail/blfs-dev/2013-August/025629.html
#sed -i  -e '/YYLEX_PARAM/d'                                       \
#         -e '/parse-param.*scanner/i %lex-param { void *scanner }' \
#     gst/parse/grammar.y
#./configure --prefix=${GSTREAMER_0_10_INSTALL_PREFIX} --enable-static --disable-shared
#make
#make install
#cd "${BASE_DIR}"

################################################################################

#GSTREAMER_0_10_GST_PLUGINS_BASE_RELEASE="0.10.36"
#GSTREAMER_0_10_GST_PLUGINS_BASE_NAME="gst-plugins-base"
#GSTREAMER_0_10_GST_PLUGINS_BASE_URL="http://gstreamer.freedesktop.org/src/${GSTREAMER_0_10_GST_PLUGINS_BASE_NAME}/${GSTREAMER_0_10_GST_PLUGINS_BASE_NAME}-${GSTREAMER_0_10_GST_PLUGINS_BASE_RELEASE}.tar.bz2"
#GSTREAMER_0_10_GST_PLUGINS_BASE_SRC="${GSTREAMER_0_10_GST_PLUGINS_BASE_NAME}-${GSTREAMER_0_10_GST_PLUGINS_BASE_RELEASE}"
#GSTREAMER_0_10_GST_PLUGINS_BASE_INSTALL_PREFIX="${BASE_DIR}/${INSTALL_DIR}/${GSTREAMER_0_10_GST_PLUGINS_BASE_SRC}_install"
#GSTREAMER_0_10_GST_PLUGINS_BASE_SRC_DIR="${INSTALL_DIR}/${GSTREAMER_0_10_GST_PLUGINS_BASE_SRC}"
#
#cd "${BASE_DIR}"
#mkdir -p "${INSTALL_DIR}"
#mkdir -p "${GSTREAMER_0_10_GST_PLUGINS_BASE_INSTALL_PREFIX}"
#
#cd "${INSTALL_DIR}"
#wget "${GSTREAMER_0_10_GST_PLUGINS_BASE_URL}"
#tar xvjf "${GSTREAMER_0_10_GST_PLUGINS_BASE_SRC}.tar.bz2"
#cd "${BASE_DIR}"
#
#cd "${GSTREAMER_0_10_GST_PLUGINS_BASE_SRC_DIR}"
#./configure --prefix=${GSTREAMER_0_10_GST_PLUGINS_BASE_INSTALL_PREFIX} --enable-static --disable-shared
#make
#make install
#cd "${BASE_DIR}"


################################################################################

#P11_KIT_RELEASE="0.20.2"
#P11_KIT_NAME="p11-kit"
#P11_KIT_EXT="tar.gz"
#P11_KIT_URL="http://p11-glue.freedesktop.org/releases/${P11_KIT_NAME}-${P11_KIT_RELEASE}.${P11_KIT_EXT}"
#P11_KIT_SRC="${P11_KIT_NAME}-${P11_KIT_RELEASE}"
#P11_KIT_INSTALL_PREFIX="${BASE_DIR}/${INSTALL_DIR}/${P11_KIT_SRC}_install"
#P11_KIT_SRC_DIR="${INSTALL_DIR}/${P11_KIT_SRC}"
#
#cd "${BASE_DIR}"
#mkdir -p "${INSTALL_DIR}"
#mkdir -p "${P11_KIT_INSTALL_PREFIX}"
#
#cd "${INSTALL_DIR}"
#wget "${P11_KIT_URL}"
#tar xvzf "${P11_KIT_SRC}.${P11_KIT_EXT}"
#cd "${BASE_DIR}"
#
#cd "${P11_KIT_SRC_DIR}"
#./configure --prefix=${P11_KIT_INSTALL_PREFIX} --enable-static --disable-shared
#make
#make install
#cd "${BASE_DIR}"


################################################################################

LIBAV_RELEASE="9.16"
LIBAV_NAME="libav"
LIBAV_EXT="tar.gz"
LIBAV_URL="http://libav.org/releases/${LIBAV_NAME}-${LIBAV_RELEASE}.${LIBAV_EXT}"
LIBAV_SRC="${LIBAV_NAME}-${LIBAV_RELEASE}"
LIBAV_INSTALL_PREFIX="${BASE_DIR}/${INSTALL_DIR}/${LIBAV_SRC}_install"
LIBAV_SRC_DIR="${INSTALL_DIR}/${LIBAV_SRC}"

cd "${BASE_DIR}"
mkdir -p "${INSTALL_DIR}"
mkdir -p "${LIBAV_INSTALL_PREFIX}"

cd "${INSTALL_DIR}"
# wget "${LIBAV_URL}"
# tar xvzf "${LIBAV_SRC}.${LIBAV_EXT}"
cd "${BASE_DIR}"

cd "${LIBAV_SRC_DIR}"
./configure --prefix=${LIBAV_INSTALL_PREFIX} --enable-static --disable-shared \
 --enable-libx264 \
 --enable-libxvid \
 --enable-gpl \
 --disable-network
make
make install
cd "${BASE_DIR}"

