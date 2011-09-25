#!/bin/bash

if [ -z $UDT_ARCH ]; then
UDT_ARCH=IA64
fi

if [ -z $UDT_OS ]; then
UDT_OS=LINUX
fi

echo "building UDT with OS=${UDT_OS} ARCH=${UDT_ARCH}"

pushd deps/UDT4
make -e os=$UDT_OS arch=$UDT_ARCH
popd

pushd src
cmake .
make 
popd