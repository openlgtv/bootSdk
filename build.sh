#!/bin/bash
set -e

[ ! -d build ] && mkdir build
cd build

cmake .. \
	-DCMAKE_C_COMPILER="$(which ${CROSS_COMPILE}gcc)" \
	-DCMAKE_OBJCOPY="$(which ${CROSS_COMPILE}objcopy)"

cmake --build .
