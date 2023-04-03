@echo off

REM Installing vcpkg
git clone https://github.com/microsoft/vcpkg.git
git submodule init
git submodule update 
echo Added vckpg as submodule

REM Running bootstrap script
cd vcpkg
bootstrap-vcpkg.bat
echo bootstrap complete

PAUSE