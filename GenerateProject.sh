#!/bin/bash

powershell kill -n devenv

mkdir -p build
cd build
cmake ..

start SDLBgfxTestProject.sln