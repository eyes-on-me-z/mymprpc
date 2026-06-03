#!/bin/bash

set -e

PROJECT_DIR=$(cd "$(dirname "$0")"; pwd)

mkdir -p "${PROJECT_DIR}/build"

rm -rf "${PROJECT_DIR}/build"/*

cd "${PROJECT_DIR}/build"

cmake ..
make -j$(nproc)

rm -rf "${PROJECT_DIR}/lib/include"
cp -r "${PROJECT_DIR}/src/include" "${PROJECT_DIR}/lib"