rm -rf thirdparty
rm -rf install
rm -rf build
cmake -S . -B build
cmake --build build
