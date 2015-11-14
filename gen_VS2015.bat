@echo off
pushd .
cd ..
rd /s/q CNum_VS2015
mkdir CNum_VS2015
cd CNum_VS2015
cmake ../CNum -G"Visual Studio 14 2015 Win64"
popd
pause