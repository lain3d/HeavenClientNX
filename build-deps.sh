#!/bin/bash

cd libs
source /opt/devkitpro/switchvars.sh
cd NoLifeNx/nlnx/lz4-1.8.2
make -f Makefile.nx && \
cd ../ && ./build-switch.sh && \
cd ../../glad && make && \
cd ../glfw && ./build.sh && \
cd ../openal-soft && make -f Makefile.nx && \
cd ../alure && ./build-switch.sh 

