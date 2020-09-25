#!/bin/bash

mkdir -p build/SDLBgfxTest/shaders
mkdir -p build/SDLBgfxTest/shaders/dx9
mkdir -p build/SDLBgfxTest/shaders/dx11

# DX9
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/dx9/sprite.vs.bin" --platform windows -p vs_3_0 -O 3 --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/dx9/sprite.fs.bin" --platform windows -p ps_3_0 -O 3 --type fragment --verbose -i bgfx/bgfx/src

# DX11
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/dx11/sprite.vs.bin" --platform windows -p vs_5_0 -O 3 --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/dx11/sprite.fs.bin" --platform windows -p ps_5_0 -O 3 --type fragment --verbose -i bgfx/bgfx/src