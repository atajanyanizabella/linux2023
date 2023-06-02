#!/bin/bash

MIN_QT_VERSION=5.15.3
INSTALLED=false

# Check if qmake is available
if ! command -v qmake &> /dev/null
then
    echo "qmake not found, installing Qt $MIN_QT_VERSION..."
    sudo apt-get update
    sudo apt-get install qtbase5-dev qt5-qmake
    INSTALLED=true
else
    # Get the Qt version from qmake
    QT_VERSION=$(qmake -v | awk '/Using Qt version/ {print $NF}')

    # Compare the Qt version with the minimum required version
    if [ "$(printf '%s\n' "$MIN_QT_VERSION" "$QT_VERSION" | sort -V | head -n1)" != "$MIN_QT_VERSION" ]
    then
        echo "Qt version $QT_VERSION is not supported, installing Qt $MIN_QT_VERSION..."
        sudo apt-get update
        sudo apt-get install qtbase5-dev qt5-qmake
        INSTALLED=true
    fi
fi

# Check if make is available
if ! command -v make &> /dev/null
then
    echo "make not found, installing..."
    sudo apt-get update
    sudo apt-get install make
    INSTALLED=true
fi

# Check if nlohmann::json is installed
if ! dpkg -s nlohmann-json3-dev &> /dev/null
then
    echo "nlohmann-json3-dev not found, installing..."
    sudo apt-get update
    sudo apt-get install nlohmann-json3-dev
    INSTALLED=true
fi

if [ "$INSTALLED" = true ]
then
    echo "Installation complete"
else
    echo "Already installed"
fi
