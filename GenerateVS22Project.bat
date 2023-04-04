@echo off

REM Installing assimp
cd vcpkg
vcpkg install assimp:x64-windows --recurse
::vcpkg install spdlog:x64-windows --recurse
::vcpkg install imgui:x64-windows --recurse
echo Installed assimp 
echo To generate project,
PAUSE

cd ..

REM Generate Solution
cmake -G "Visual Studio 17 2022" -A=x64 -B=./build/x64/Debug -DCMAKE_ARCHITECTURE=x64 -DCMAKE_BUILD_TYPE=Debug
REM Build Binaries
cmake --build build/x64/Debug --config Debug

copy "vcpkg\installed\x64-windows\debug\bin\assimp-vc143-mtd.dll" "build\x64\Debug\bin"
PAUSE