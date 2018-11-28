#!/usr/bin/env bash

unameOut="$(uname -s)"

if [ "$unameOut" = "Linux" ]; then
    sudo apt install python3-pip
    sudo pip3 install conan


    conan remote add r-Type https://api.bintray.com/conan/bincrafters/public-conan

    mkdir build
    cd build

    conan install .. --build=missing
    cmake .. -G "Unix Makefiles"
    make
elif [ "$unameOut" = "Darwin" ]; then
    echo "MAC OS"
fi