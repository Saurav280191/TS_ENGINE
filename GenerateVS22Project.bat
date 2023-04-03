@echo off

REM Installing assimp and spdlog
cd vcpkg
vcpkg install assimp:x64-windows --recurse
vcpkg install spdlog:x64-windows --recurse
echo Installed assimp, imgui, and spdlog. 
echo To generate project,
PAUSE

cd ..

REM Generate Solution
cmake -G "Visual Studio 17 2022" -A=x64 -B=./build/x64/Debug -DCMAKE_ARCHITECTURE=x64 -DCMAKE_BUILD_TYPE=Debug
REM Build Binaries
cmake --build build/x64/Debug --config Debug

PAUSE