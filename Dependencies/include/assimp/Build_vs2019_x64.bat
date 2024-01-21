SET SOURCE_DIR=.
SET GENERATOR=Visual Studio 16 2019

SET BINARIES_DIR="./build/x64"
cmake . -G "%GENERATOR%" -DASSIMP_NO_EXPORT=ON -A x64 -S %SOURCE_DIR% -B %BINARIES_DIR%
cmake --build %BINARIES_DIR% --config debug
cmake --build %BINARIES_DIR% --config release

echo Assimp build complete
PAUSE
