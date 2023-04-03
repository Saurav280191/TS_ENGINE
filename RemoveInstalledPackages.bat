@echo off

REM Installing vcpkg
cd vcpkg
vcpkg remove --recurse assimp:x64-windows
vcpkg remove --recurse spdlog:x64-windows

echo Uninstalled vcpkgs 
PAUSE