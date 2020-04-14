#!/usr/bin/env bash

# Setup
# Exit the script if any command fails
set -e


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
cmake -DJSON_BuildTests=false ..
make -j$(nproc)
sudo make install


# nlohmann json
cd /tmp
git clone --depth 1 https://github.com/nlohmann/json.git
cd json
mkdir build && cd build
cmake -DJSON_BuildTests=false ..
make -j$(nproc)
sudo make install


sudo ldconfig