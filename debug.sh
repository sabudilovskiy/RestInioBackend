conan install . --build=missing --install-folder conan_libraries --profile debug
mkdir build
cd build
mkdir debug
cd debug
cmake ../..
