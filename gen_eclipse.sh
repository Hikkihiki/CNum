rm -rf ../CNum_Eclipse
mkdir ../CNum_Eclipse
pushd ../CNum_Eclipse
cmake ../CNum -DCMAKE_CXX_COMPILER=g++ -G"Eclipse CDT4 - Unix Makefiles"
popd
