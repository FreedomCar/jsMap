sudo apt install build-essential git cmake pkg-config \
libbz2-dev libxml2-dev libzip-dev libboost-all-dev \
lua5.2 liblua5.2-dev libtbb-dev
mkdir -p build
cd build
cmake ..
cmake --build .
sudo cmake --build . --target install
cd ..
make
cd test/data
make
cd ..
cd ..
./osrm-example test/data/monaco.osrm