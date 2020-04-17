#!/usr/bin/env bash

# Setup
# Exit the script if any command fails
set -e


# Dependencies from LibCommon
./extern/LibCommon/installDependencies.sh


# WebsocketCPP
# Libwebsockets
sudo apt install libssl-dev
cd /tmp
git clone https://github.com/warmcat/libwebsockets.git
cd libwebsockets
mkdir build && cd build
cmake -DJSON_BuildTests=false ..
make -j$(nproc)
sudo make install
sudo ldconfig
# WebsocketCPP
cd /tmp
git clone https://github.com/SoPra-Team-17/WebsocketCPP.git
cd WebsocketCPP
mkdir build && cd build
cmake ..
make -j$(nproc)
sudo make install


sudo ldconfig