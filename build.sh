#!/bin/bash
#if [ "$1" = "clean" ]; then
echo "Deleting and recreating the build directory "
rm -rf build
mkdir build
#fi

cd build
#export PKG_CONFIG_PATH=/opt/local/argos3/2.8.12.2/gcc/5.4.0/lib/pkgconfig
echo "Configuring Makefiles with CMAKE..."
cmake .. \
  -DBUILD_EVOLVER=YES \
  -DCMAKE_BUILD_TYPE=Release \
  -DCMAKE_POLICY_VERSION_MINIMUM=3.5 \
  -DBoost_NO_BOOST_CMAKE=ON \
  -DBOOST_ROOT=/opt/homebrew/opt/boost \
  -DCMAKE_PREFIX_PATH=/opt/homebrew/opt/boost

echo "Making..."
make

cd ..

echo "Finished."
