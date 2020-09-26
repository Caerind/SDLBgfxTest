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

includepath="bgfx/bgfx/src"
allfsfiles="`find SDLBgfxTest/shaders -name *.fs`"
for fsfile in $allfsfiles
do
    basename="`echo $fsfile | cut -f 1 -d '.'`"
    vsfile="${basename}.vs"
    fsbin="build/${fsfile}.bin"
    vsbin="build/${vsfile}.bin"

    #TODO: platform asm.js ?

    # DX9
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform windows -p vs_3_0 -O 3 --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform windows -p ps_3_0 -O 3 --type fragment --verbose -i $includepath

    # DX11
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform windows -p vs_5_0 -O 3 --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform windows -p ps_5_0 -O 3 --type fragment --verbose -i $includepath

    # NACL
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform nacl --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform nacl --type fragment --verbose -i $includepath

    # Android
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform android --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform android --type fragment --verbose -i $includepath

    # GLSL
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform linux -p 120 --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform linux -p 120 --type fragment --verbose -i $includepath

    # Metal
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform osx -p metal --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform osx -p metal --type fragment --verbose -i $includepath

    # PSSL
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform orbis -p pssl --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform orbis -p pssl --type fragment --verbose -i $includepath

    # Spirv
    ${path_shaderc} -f "${vsfile}" -o "${vsbin}" --platform linux -p spirv --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "${fsbin}" --platform linux -p spirv --type fragment --verbose -i $includepath
done