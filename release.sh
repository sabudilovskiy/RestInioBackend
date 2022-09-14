conan install . --build=missing --install-folder conan_libraries --profile release
mkdir build
cd build
mkdir release
cd release
cmake ../..
