REM Install CMake if not installed
@echo off
where cmake >nul 2>nul
if %errorlevel% == 0 (
    echo CMake is already installed
) else (
    echo Installing CMake...
    choco install cmake -y
)

REM Generate Solution
cmake -G "Visual Studio 17 2022" -A=x64 -B=./build/x64/Debug -DCMAKE_ARCHITECTURE=x64 -DCMAKE_BUILD_TYPE=Debug
REM Build Binaries
cmake --build build/x64/Debug --config Debug

copy "ThirdParty\assimp\build\x64\bin\Debug\assimp-vc142-mtd.dll" "build\x64\Debug\bin"

