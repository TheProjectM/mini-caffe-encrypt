#!/usr/bin/env bash

./generatepb.sh
rm -rf build_gpu/ build_cpu/

echo "building mini-caffe gpu version"
mkdir build_gpu
cd build_gpu
cmake .. -DCMAKE_BUILD_TYPE=Release -DUSE_CUDA=ON -DUSE_CUDNN=ON -DCUDA_ARCH_NAME=All
make -j$(nproc)
echo "mini-caffe gpu version built to $(pwd)"
cp ../example/models/ssd/MobileNetSSD_deploy.prototxt .
echo "test mini-caffe gpu version speed"
./benchmark MobileNetSSD_deploy.prototxt 8 0

echo "building mini-caffe cpu version"
mkdir ../build_cpu
cd ../build_cpu
cmake .. -DCMAKE_BUILD_TYPE=Release
make -j$(nproc)
echo "mini-caffe cpu version built to $(pwd)"
cp ../example/models/ssd/MobileNetSSD_deploy.prototxt .
echo "test mini-caffe cpu version speed"
./benchmark MobileNetSSD_deploy.prototxt 8 -1
