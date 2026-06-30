# PSP-UFU Linux Build and Packaging Guide

> This document describes how to build, install, package, and distribute PSP-UFU on Linux using WSL and Ubuntu.

---

# Repository

Official repository:

```text
https://github.com/Thales1330/PSP.git
```

Clone the repository:

```bash
git clone https://github.com/Thales1330/PSP.git
```

Enter the repository:

```bash
cd PSP
```

Throughout this guide, all commands assume you are inside the repository root directory:

```text
PSP/
├── CMakeLists.txt
├── Project/
├── vendors/
├── docs/
└── README.md
```

---

# Prerequisites

This guide was validated using:

* Windows 11
* WSL2
* Ubuntu 24.04 LTS

---

# Installing WSL

Open PowerShell as Administrator and run:

```powershell
wsl --install
```

Reboot Windows if requested.

Install Ubuntu from the Microsoft Store.

---

# Updating Ubuntu

Open Ubuntu and update the system:

```bash
sudo apt update
sudo apt upgrade -y
```

---

# Installing Build Dependencies

Install all required packages:

```bash
sudo apt install -y \
build-essential \
cmake \
git \
wget \
fakeroot \
dpkg-dev \
patchelf \
desktop-file-utils \
libfuse2 \
libgtk-3-dev \
libgl1-mesa-dev \
libfftw3-dev
```

Verify CMake installation:

```bash
cmake --version
```

Verify Git installation:

```bash
git --version
```

---

# Obtaining the Repository

## First Installation

Clone the repository:

```bash
git clone https://github.com/Thales1330/PSP.git
```

Enter the repository:

```bash
cd PSP
```

Verify:

```bash
ls
```

Expected output:

```text
CMakeLists.txt
Project
vendors
README.md
```

---

## Updating an Existing Repository

If the repository already exists locally, do not clone it again.

Enter the repository:

```bash
cd ~/PSP
```

Check repository status:

```bash
git status
```

Update local files:

```bash
git pull
```

If submodules are added in the future:

```bash
git submodule update --init --recursive
```

---

# Cleaning Previous Builds

From the repository root:

```bash
rm -rf build
rm -rf install
rm -rf deb
rm -rf AppDir
rm -rf squashfs-root
```

---

# Configuring the Project

Verify that the current directory contains the CMakeLists file:

```bash
ls CMakeLists.txt
```

Expected:

```text
CMakeLists.txt
```

Configure the project in Release mode:

```bash
cmake -S . -B build -DCMAKE_BUILD_TYPE=Release
```

---

# Building PSP-UFU

Compile:

```bash
cmake --build build -j8
```

Generated executable:

```text
build/bin/PSP-UFU
```

---

# Testing the Build

Run:

```bash
./build/bin/PSP-UFU
```

Verify:

* Application starts correctly
* Menus are displayed correctly
* Charts are rendered correctly
* Data files are loaded correctly

Close the application before proceeding.

---

# Creating a Local Installation

Install PSP-UFU into a local directory:

```bash
cmake --install build --prefix install
```

Expected structure:

```text
install
├── bin
│   └── PSP-UFU
├── lib
│   └── psp-ufu
│       ├── libchartdir.so.7.0
│       └── libchartdir.so.7.0.0
└── share
    └── psp-ufu
        └── data
```

---

# Testing the Local Installation

Run:

```bash
./install/bin/PSP-UFU
```

Verify:

* Application starts correctly
* ChartDirector loads correctly
* The data directory is found automatically

If everything works correctly, proceed to packaging.

---

# Creating a Debian Package

Create package directories:

```bash
mkdir -p deb/psp-ufu/DEBIAN
mkdir -p deb/psp-ufu/usr
```

Install PSP-UFU into the package structure:

```bash
cmake --install build --prefix deb/psp-ufu/usr
```

---

## Creating the Control File

Create:

```bash
nano deb/psp-ufu/DEBIAN/control
```

Contents:

```text
Package: psp-ufu
Version: 1.0
Section: science
Priority: optional
Architecture: amd64
Maintainer: PSP-UFU Team
Description: Power System Platform UFU
 Power system simulation software.
```

Save and exit.

---

## Building the Debian Package

Enter the package directory:

```bash
cd deb
```

Build:

```bash
dpkg-deb --build psp-ufu
```

Generated package:

```text
psp-ufu.deb
```

Return to repository root:

```bash
cd ..
```

---

## Testing the Debian Package

Install:

```bash
sudo apt install ./deb/psp-ufu.deb
```

Run:

```bash
PSP-UFU
```

Verify normal operation.

---

# Creating an AppImage

Create AppDir:

```bash
mkdir -p AppDir/usr
```

Install PSP-UFU into AppDir:

```bash
cmake --install build --prefix AppDir/usr
```

---

## Creating AppRun

Create:

```bash
nano AppDir/AppRun
```

Contents:

```bash
#!/bin/bash

HERE="$(dirname "$(readlink -f "$0")")"

export LD_LIBRARY_PATH="$HERE/usr/lib/psp-ufu:$LD_LIBRARY_PATH"

exec "$HERE/usr/bin/PSP-UFU" "$@"
```

Save and exit.

Make executable:

```bash
chmod +x AppDir/AppRun
```

---

## Creating Desktop Entry

Create:

```bash
nano AppDir/psp-ufu.desktop
```

Contents:

```ini
[Desktop Entry]
Type=Application
Name=PSP-UFU
Exec=PSP-UFU
Icon=psp-ufu
Categories=Science;Engineering;
Terminal=false
```

Save and exit.

---

## Adding an Icon

Copy a PNG icon to the AppDir root:

```bash
cp <icon-file>.png AppDir/psp-ufu.png
```

Example:

```bash
cp Project/data/images/psp-ufu.png AppDir/psp-ufu.png
```

Expected structure:

```text
AppDir
├── AppRun
├── psp-ufu.desktop
├── psp-ufu.png
└── usr
    ├── bin
    ├── lib
    └── share
```

---

# Downloading AppImageTool

Download:

```bash
wget https://github.com/AppImage/AppImageKit/releases/download/continuous/appimagetool-x86_64.AppImage
```

Make executable:

```bash
chmod +x appimagetool-x86_64.AppImage
```

---

# Generating the AppImage

Generate:

```bash
./appimagetool-x86_64.AppImage AppDir
```

Generated file:

```text
PSP-UFU-x86_64.AppImage
```

---

# Testing the AppImage

Make executable:

```bash
chmod +x PSP-UFU-x86_64.AppImage
```

Run:

```bash
./PSP-UFU-x86_64.AppImage
```

Verify:

* Application starts correctly
* Data directory is found
* ChartDirector loads correctly
* All features work normally

---

# Distribution Files

The final files intended for distribution are:

```text
deb/psp-ufu.deb
PSP-UFU-x86_64.AppImage
```

---

# Supported Linux Distributions

## Debian Package (.deb)

Compatible with:

* Ubuntu
* Debian
* Linux Mint
* Pop!_OS
* KDE Neon
* Elementary OS
* Zorin OS
* Deepin

## AppImage

Compatible with most modern Linux distributions:

* Ubuntu
* Debian
* Fedora
* Linux Mint
* openSUSE
* Arch Linux
* Pop!_OS

Provided that FUSE support is available.