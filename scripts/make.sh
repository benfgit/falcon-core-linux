#!/bin/sh
set -e
cd build
make -j$(nproc 2>/dev/null || sysctl -n hw.logicalcpu)
mkdir -p debug
cp core/falcon debug/
cp core/tools1/nlxtestbench/nlxtestbench debug/
