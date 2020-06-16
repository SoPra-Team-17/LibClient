#!/usr/bin/env bash

# Setup
# Exit the script if any command fails
set -e


# Dependencies from LibCommon
./extern/LibCommon/installDependencies.sh


# Dependencies from WebsocketCPP
./extern/WebsocketCPP/installDependencies.sh


sudo ldconfig