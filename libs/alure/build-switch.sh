#!/bin/bash

BASEDIR=$(dirname "$0")
NCPU=`cat /proc/cpuinfo |grep vendor_id |wc -l`
let NCPU=8
echo "Will build with 'make -j$NCPU' ... please edit this script if incorrect."

set -e
set -x

rm -rf switch-build
mkdir $_
cd $_

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

