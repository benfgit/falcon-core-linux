#!/bin/sh
set -e
cd build
make -j$(nproc 2>/dev/null || sysctl -n hw.logicalcpu)
mkdir -p debug
cp falcon/falcon debug/
cp falcon/tools0/nlxtestbench/nlxtestbench debug/
cp ../falcon-fklab-extensions/tools/nlxtestbench/test_configs/nlxtestbench.yaml debug/