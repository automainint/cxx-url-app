rm -rf install
rm -rf build
cmake -D URLAPP_BUILD_DEPENDENCIES=OFF -S . -B build
cmake --build build
