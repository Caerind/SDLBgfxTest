#!/bin/bash

platform='unknown'
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
	platform='linux'
elif [[ "$OSTYPE" == "freebsd"* ]]; then
	platform='bsd'
elif [[ "$OSTYPE" == "darwin"* ]]; then
	platform='mac'
elif [[ "$OSTYPE" == "cygwin" ]]; then
	platform='windows'
elif [[ "$OSTYPE" == "msys" ]]; then
	platform='windows'
elif [[ "$OSTYPE" == "win32" ]]; then
	platform='windows'
else
	echo "OSTYPE == ${OSTYPE} not reconized, assuming it's windows based"
	platform='windows'
fi

if [[ "$platform" == "windows" ]]; then
    path_shaderc="build/bgfx/Debug/shaderc.exe" # default path for Visual Studio users
else
    path_shaderc="./build/bgfx/shaderc" # for Linux/Unix GCC users
fi

mkdir -p build/SDLBgfxTest/shaders
mkdir -p build/SDLBgfxTest/shaders/dx9
mkdir -p build/SDLBgfxTest/shaders/dx11
mkdir -p build/SDLBgfxTest/shaders/essl
mkdir -p build/SDLBgfxTest/shaders/glsl
mkdir -p build/SDLBgfxTest/shaders/metal
mkdir -p build/SDLBgfxTest/shaders/pssl
mkdir -p build/SDLBgfxTest/shaders/spirv

# DX9
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/dx9/sprite.vs.bin" --platform windows -p vs_3_0 -O 3 --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/dx9/sprite.fs.bin" --platform windows -p ps_3_0 -O 3 --type fragment --verbose -i bgfx/bgfx/src

# DX11
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/dx11/sprite.vs.bin" --platform windows -p vs_5_0 -O 3 --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/dx11/sprite.fs.bin" --platform windows -p ps_5_0 -O 3 --type fragment --verbose -i bgfx/bgfx/src

# NACL
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/essl/sprite.vs.bin" --platform nacl --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/essl/sprite.fs.bin" --platform nacl --type fragment --verbose -i bgfx/bgfx/src

# Android
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/essl/sprite.vs.bin" --platform android --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/essl/sprite.fs.bin" --platform android --type fragment --verbose -i bgfx/bgfx/src

# GLSL
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/glsl/sprite.vs.bin" --platform linux -p 120 --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/glsl/sprite.fs.bin" --platform linux -p 120 --type fragment --verbose -i bgfx/bgfx/src

# Metal
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/metal/sprite.vs.bin" --platform osx -p metal --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/metal/sprite.fs.bin" --platform osx -p metal --type fragment --verbose -i bgfx/bgfx/src

# PSSL
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/pssl/sprite.vs.bin" --platform orbis -p pssl --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/pssl/sprite.fs.bin" --platform orbis -p pssl --type fragment --verbose -i bgfx/bgfx/src

# Spirv
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/spirv/sprite.vs.bin" --platform linux -p spirv --type vertex --verbose -i bgfx/bgfx/src
${path_shaderc} -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/spirv/sprite.fs.bin" --platform linux -p spirv --type fragment --verbose -i bgfx/bgfx/src