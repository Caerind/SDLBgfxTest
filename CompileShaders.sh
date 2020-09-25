#!/bin/bash

mkdir -p build/SDLBgfxTest/shaders
mkdir -p build/SDLBgfxTest/shaders/dx9
mkdir -p build/SDLBgfxTest/shaders/dx11
mkdir -p build/SDLBgfxTest/shaders/essl
mkdir -p build/SDLBgfxTest/shaders/glsl
mkdir -p build/SDLBgfxTest/shaders/metal
mkdir -p build/SDLBgfxTest/shaders/pssl
mkdir -p build/SDLBgfxTest/shaders/spirv

# DX9
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/dx9/sprite.vs.bin" --platform windows -p vs_3_0 -O 3 --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/dx9/sprite.fs.bin" --platform windows -p ps_3_0 -O 3 --type fragment --verbose -i bgfx/bgfx/src

# DX11
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/dx11/sprite.vs.bin" --platform windows -p vs_5_0 -O 3 --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/dx11/sprite.fs.bin" --platform windows -p ps_5_0 -O 3 --type fragment --verbose -i bgfx/bgfx/src

# NACL
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/essl/sprite.vs.bin" --platform nacl --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/essl/sprite.fs.bin" --platform nacl --type fragment --verbose -i bgfx/bgfx/src

# Android
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/essl/sprite.vs.bin" --platform android --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/essl/sprite.fs.bin" --platform android --type fragment --verbose -i bgfx/bgfx/src

# GLSL
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/glsl/sprite.vs.bin" --platform linux -p 120 --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/glsl/sprite.fs.bin" --platform linux -p 120 --type fragment --verbose -i bgfx/bgfx/src

# Metal
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/metal/sprite.vs.bin" --platform osx -p metal --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/metal/sprite.fs.bin" --platform osx -p metal --type fragment --verbose -i bgfx/bgfx/src

# PSSL
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/pssl/sprite.vs.bin" --platform orbis -p pssl --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/pssl/sprite.fs.bin" --platform orbis -p pssl --type fragment --verbose -i bgfx/bgfx/src

# Spirv
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/spirv/sprite.vs.bin" --platform linux -p spirv --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/spirv/sprite.fs.bin" --platform linux -p spirv --type fragment --verbose -i bgfx/bgfx/src