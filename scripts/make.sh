#!/bin/sh
set -e
cd build
make -j$(nproc 2>/dev/null || sysctl -n hw.logicalcpu)
mkdir -p debug
cp core/falcon debug/ 2>/dev/null
cp core/tools0/nlxtestbench/nlxtestbench debug/ 2>/dev/null
