#!/bin/bash

# This script installs the required dependencies for the project.

set -e  # Exit immediately if a command exits with a non-zero status.

# Function to install dependencies for Debian/Ubuntu-based systems
install_debian() {
    echo "Installing dependencies for Debian/Ubuntu..."
    sudo apt-get update
    sudo apt-get install -y libavcodec-dev libavformat-dev libswscale-dev libavutil-dev libgtest-dev
}

# Function to install dependencies for RHEL/CentOS-based systems
install_rhel() {
    echo "Installing dependencies for RHEL/CentOS..."
    sudo yum install -y ffmpeg-devel gtest-devel
}

# Function to install dependencies for Arch Linux
install_arch() {
    echo "Installing dependencies for Arch Linux..."
    sudo pacman -Sy --noconfirm ffmpeg googletest
}

# Check for package manager and install dependencies accordingly
if command -v apt-get &> /dev/null; then
    install_debian
elif command -v yum &> /dev/null; then
    install_rhel
elif command -v pacman &> /dev/null; then
    install_arch
else
    echo "Unsupported package manager. Please install dependencies manually."
    exit 1
fi

# Optional: Build GTest if not installed (uncomment if necessary)
# echo "Building GoogleTest..."
# cd /usr/src/gtest
# sudo cmake .
# sudo cp *.a /usr/lib

echo "Dependencies installed successfully!"
