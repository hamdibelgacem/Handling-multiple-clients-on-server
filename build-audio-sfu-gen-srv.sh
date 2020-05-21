INSTALL_DIR=$PWD/install
echo $INSTALL_DIR
mkdir -p build
cd build
cmake -DCMAKE_INSTALL_PREFIX=$INSTALL_DIR ..
make
cmake --build . --target install

export PATH=$PATH:$INSTALL_DIR/bin
