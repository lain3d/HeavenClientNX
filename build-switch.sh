#!/bin/bash

BASEDIR=$(dirname "$0")
echo ${BASEDIR}
NCPU=`cat /proc/cpuinfo |grep vendor_id |wc -l`
let NCPU=10
echo "Will build with 'make -j$NCPU' ... please edit this script if incorrect."

#set -e
#set -x

rm -rf switch-build
mkdir $_
cd $_
cd switch_build

# IMPORTANT: run another build script first to at least get a ecc binary for your host platform

source "$DEVKITPRO/switchvars.sh"
cmake -G"Unix Makefiles" \
  -DCMAKE_TOOLCHAIN_FILE="$DEVKITPRO/switch.cmake" \
  -DCMAKE_BUILD_TYPE=RelWithDebInfo \
  -DPKG_CONFIG_EXECUTABLE="$DEVKITPRO/portlibs/switch/bin/aarch64-none-elf-pkg-config" \
  -DECC="$BASEDIR/cmake-build/ecc" \
  -DUSE_ASM=FALSE \
  $1 \
  ..

make -j$NCPU
