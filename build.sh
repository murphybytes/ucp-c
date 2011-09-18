#!/bin/bash

ARCHITECTURE=IA64
OS=LINUX

pushd deps/UDT4
make -e os=$OS arch=$ARCHITECURE
popd

pushd src
cmake .
make 
popd