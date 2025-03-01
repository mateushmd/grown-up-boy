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
cmake -DCMAKE_BUILD_TYPE=Debug -DCMAKE_PREFIX_PATH=%~dp0/_vendor/sdl3-src/build .. -G Ninja

echo Building the project...
REM ninja > build.log 2>&1  
ninja

cd ..

echo Build process complete.