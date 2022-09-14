wget  -O ./conan.deb "https://github.com/conan-io/conan/releases/latest/download/conan-ubuntu-64.deb"
sudo dpkg -i conan.deb
sudo apt install g++
sudo apt install cmake
sudo apt install postgresql postgresql-contrib
sudo systemctl start postgresql.service
sudo -u postgres psql

{"connection_info" : "host=localhost user=postgres dbname=yandex"} > config.json

sudo update-rc.d postgresql enable

cp -R ./profiles ~/.conan/profiles

source debug.sh