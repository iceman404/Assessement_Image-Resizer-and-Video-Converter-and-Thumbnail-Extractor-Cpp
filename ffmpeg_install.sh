#!/bin/bash

# This script uninstalls the forked version of libav and installs the required dependencies for the project.

set -e  # Exit immediately if a command exits with a non-zero status.

# Function to uninstall older libav fork on Debian/Ubuntu
uninstall_libav_debian() {
    echo "Uninstalling libav on Debian/Ubuntu..."
    sudo apt-get remove --purge -y libavcodec-dev libavformat-dev libswscale-dev libavutil-dev
    sudo apt-get autoremove -y
}

# Function to uninstall older libav fork on RHEL/CentOS
uninstall_libav_rhel() {
    echo "Uninstalling libav on RHEL/CentOS..."
    sudo yum remove -y libavcodec libavformat libavutil libswscale
}

# Function to uninstall older libav fork on Arch Linux
uninstall_libav_arch() {
    echo "Uninstalling libav on Arch Linux..."
    sudo pacman -Rns --noconfirm libavcodec libavformat libavutil libswscale
}

# Function to install dependencies for Debian/Ubuntu-based systems
install_debian() {
    echo "Installing dependencies for Debian/Ubuntu..."
    sudo apt-get update
    sudo apt-get install -y ffmpeg libgtest-dev
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

# Check for package manager and uninstall libav accordingly
if command -v apt-get &> /dev/null; then
    uninstall_libav_debian
    install_debian
elif command -v yum &> /dev/null; then
    uninstall_libav_rhel
    install_rhel
elif command -v pacman &> /dev/null; then
    uninstall_libav_arch
    install_arch
else
    echo "Unsupported package manager. Please uninstall libav manually and install ffmpeg."
    exit 1
fi

# Optional: Build GTest if not installed (uncomment if necessary)
# echo "Building GoogleTest..."
# cd /usr/src/gtest
# sudo cmake .
# sudo cp *.a /usr/lib

echo "Older libav uninstalled and dependencies installed successfully!"
