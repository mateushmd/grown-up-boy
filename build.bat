@echo off

set BUILD_DIR=build

if exist "%BUILD_DIR%" (
    echo Clearing build directory...
    rmdir /s /q "%BUILD_DIR%"
)

echo Creating build directory...
mkdir "%BUILD_DIR%"

cd "%BUILD_DIR%"

echo Generating build files...
cmake .. -G Ninja

echo Building the project...
REM ninja > build.log 2>&1
ninja

cd ..

copy _vendor\sdl3-build\SDL3.dll build 

echo Build process complete.