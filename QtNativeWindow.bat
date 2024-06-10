:: Fix issue with "Run as Administrator" current dir
setlocal enableextensions
cd /d "%~dp0"


:: Add CMake & Qt SDK to PATH
set PATH=%PATH%;C:\Program Files\Cmake\bin
set PATH=%PATH%;C:\Qt\6.6.3\msvc2019_64\bin

mkdir out
cd out

:: Initialize Visual Studio projects
cmake.exe -A x64  .. || exit /B 1

:: Open solution
start QtNativeWindow.sln
