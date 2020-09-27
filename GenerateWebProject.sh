#!/bin/bash

mkdir -p "build_web"
cd "build_web"

# Note:
# You may need to add include("../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake")
# in the CMakeLists.txt

EMSDK_PATH="./emsdk"
EMSCRIPTEN_VERSION="1.38.31"
CLANG_VERSION="e${EMSCRIPTEN_VERSION}_64bit"
PYTHON_VERSION="2.7.13.1_64bit\python-2.7.13.amd64"
NODE_VERSION="8.9.1_64bit"
export PATH="${PATH};${EMSDK_PATH};${EMSDK_PATH}/upstream/share/clang;${EMSDK_PATH}/node/12.18.1_64bit/bin;${EMSDK_PATH}/python/3.7.4-pywin32_64bit;${EMSDK_PATH}/upstream/emscripten;C:\raylib\MinGW\bin:$${PATH}"
EMSCRIPTEN="${EMSDK_PATH}/upstream/emscripten"
export CC="./emsdk/upstream/emscripten/emcc"
export CXX="./emsdk/upstream/emscripten/em++"
cmake .. -DCMAKE_TOOLCHAIN_FILE=../emsdk/upstream/emscripten/cmake/Modules/Platform/Emscripten.cmake -DBX_PLATFORM_EMSCRIPTEN=True -D__EMSCRIPTEN__=True -DENABLE_EMSCRIPTEN_SINGLE_FILE=True -DEMSCRIPTEN=True