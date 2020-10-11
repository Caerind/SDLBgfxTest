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
    path_shaderc="./build/bgfx/Debug/shaderc.exe" # default path for Visual Studio users
else
    path_shaderc="./build/bgfx/shaderc" # for Linux/Unix GCC users
fi

if [ ! -x $path_shaderc ]; then
	echo "You should GenerateProject and compile it first"
	exit
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
    namewoext="`echo $fsfile | cut -f 1 -d '.'`"
    vsfile="${namewoext}.vs"
    basename="`basename $fsfile`"
    basenamewoext="`echo $basename | cut -f 1 -d '.'`"

    #TODO: platform asm.js ?

    # DX9
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/dx9/${basenamewoext}.vs.bin" "${vsbin}" --platform windows -p vs_3_0 -O 3 --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/dx9/${basenamewoext}.fs.bin" --platform windows -p ps_3_0 -O 3 --type fragment --verbose -i $includepath

    # DX11
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/dx11/${basenamewoext}.vs.bin" --platform windows -p vs_5_0 -O 3 --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/dx11/${basenamewoext}.fs.bin" --platform windows -p ps_5_0 -O 3 --type fragment --verbose -i $includepath

    # NACL
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/essl/${basenamewoext}.vs.bin" --platform nacl --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/essl/${basenamewoext}.fs.bin" --platform nacl --type fragment --verbose -i $includepath

    # Android
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/essl/${basenamewoext}.vs.bin" --platform android --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/essl/${basenamewoext}.fs.bin" --platform android --type fragment --verbose -i $includepath

    # GLSL
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/glsl/${basenamewoext}.vs.bin" --platform linux -p 120 --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/glsl/${basenamewoext}.fs.bin" --platform linux -p 120 --type fragment --verbose -i $includepath

    # Metal
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/metal/${basenamewoext}.vs.bin" --platform osx -p metal --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/metal/${basenamewoext}.fs.bin" --platform osx -p metal --type fragment --verbose -i $includepath

    # PSSL
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/pssl/${basenamewoext}.vs.bin" --platform orbis -p pssl --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/pssl/${basenamewoext}.fs.bin" --platform orbis -p pssl --type fragment --verbose -i $includepath

    # Spirv
    ${path_shaderc} -f "${vsfile}" -o "build/SDLBgfxTest/shaders/spirv/${basenamewoext}.vs.bin" --platform linux -p spirv --type vertex --verbose -i $includepath
    ${path_shaderc} -f "${fsfile}" -o "build/SDLBgfxTest/shaders/spirv/${basenamewoext}.fs.bin" --platform linux -p spirv --type fragment --verbose -i $includepath
done

