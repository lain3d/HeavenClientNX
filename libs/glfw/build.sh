#!/bin/bash

source /opt/devkitpro/switchvars.sh
mkdir build
cd build
cmake -G"Unix Makefiles" -DCMAKE_TOOLCHAIN_FILE=/opt/devkitpro/switch.cmake \
    -DCMAKE_INSTALL_PREFIX=$PORTLIBS_PREFIX \
    -DCMAKE_C_FLAGS="$CFLAGS $CPPFLAGS" \
    -DCMAKE_AR="/opt/devkitpro/devkitA64/bin/aarch64-none-elf-gcc-ar" \
    -DGLFW_BUILD_EXAMPLES:BOOL=OFF -DGLFW_BUILD_TESTS:BOOL=OFF -DGLFW_BUILD_DOCS:BOOL=OFF \
    -DGLFW_VULKAN_STATIC:BOOL=ON -DGLFW_EGL_STATIC:BOOL=ON \
    ../ && make
