@echo off

echo Building assimp
cd Dependencies\include\assimp
call Build_vs2022_x64.bat
cd ..\..\..\

REM Generate Solution
cmake -G "Visual Studio 17 2022" -A=x64 -B=./build/x64/Debug -DCMAKE_ARCHITECTURE=x64 -DCMAKE_BUILD_TYPE=Debug
REM Build Binaries
cmake --build build/x64/Debug --config Debug

copy "Dependencies\include\assimp\build\x64\bin\Debug\assimp-vc143-mtd.dll" "build\x64\Debug\bin"
echo TS_ENGINE build completed
PAUSE