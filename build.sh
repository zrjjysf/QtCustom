#!/bin/bash
set -e
BUILD_DIR="./build"
if [ ! -d "${BUILD_DIR}" ]; then
    mkdir -p ${BUILD_DIR}
fi
cmake -B ${BUILD_DIR}
cp "${BUILD_DIR}/compile_commands.json" .
make -C ${BUILD_DIR}