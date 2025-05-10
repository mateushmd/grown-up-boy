#!/usr/bin/env bash
set -e

if [ -d build ]; then
    rm -rf build
fi

mkdir build
cd build
cmake -DCMAKE_BUILD_TYPE=Debug .. -G Ninja
ninja
cd ..
