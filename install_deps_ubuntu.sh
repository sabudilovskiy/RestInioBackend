wget  -O ./conan.deb "https://github.com/conan-io/conan/releases/latest/download/conan-ubuntu-64.deb"
sudo dpkg -i conan.deb
sudo apt install g++
sudo apt install cmake
cp -R ./profiles ~/.conan/profiles
source debug.sh