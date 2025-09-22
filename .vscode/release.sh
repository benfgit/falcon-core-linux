mkdir -p build && cd build
cmake .. -DCMAKE_BUILD_TYPE=Release -DCMAKE_OSX_ARCHITECTURES=arm64
make -j$(sysctl -n hw.logicalcpu)
mkdir -p release
cp falcon/falcon release/
