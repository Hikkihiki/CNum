@echo off
rd /s/q project
mkdir project
pushd project
cmake .. -G"Visual Studio 14 2015 Win64"
popd
pause