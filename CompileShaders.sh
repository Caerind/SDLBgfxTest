#!/bin/bash

mkdir -p build/SDLBgfxTest/shaders
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.vs" -o "build/SDLBgfxTest/shaders/sprite.vs.bin" --platform windows --type vertex --verbose -i bgfx/bgfx/src
build/bgfx/Debug/shaderc.exe -f "SDLBgfxTest/shaders/sprite.fs" -o "build/SDLBgfxTest/shaders/sprite.fs.bin" --platform windows --type fragment --verbose -i bgfx/bgfx/src