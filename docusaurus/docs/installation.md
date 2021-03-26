---
id: installation
title: Installation
sidebar_label: Installation
---
import useBaseUrl from "@docusaurus/useBaseUrl";

## Download

The PSP-UFU can be downloaded from the [*last release* of the PSP-UFU repository on GitHub](https://github.com/Thales1330/PSP/releases/latest).

When accessing the link, scroll down the page until you find the * Assets *:

<div><center><img src={useBaseUrl("images/download.png")} alt="Assets download" title="Informações da Figura" /></center></div>

As the PSP-UFU is a multiplatform software, you must download the correct file:
- For the operating system [Windows](installation#windows) download the files *.exe* (recommended) or *.zip*.
- For [Linux](installation#linux) systems based on Debian (for example, Ubuntu, Mint) *64 bits* download the installation package *.deb*.

:::tip Tip
If you are interested in contributing to PSP-UFU by building it for other operating systems (mainly macOS!) Contact us at [GitHub](https://github.com/Thales1330/PSP/issues) or [Twitter](https://twitter.com/PspUfu).
:::

Following are the step-by-step instructions for installing the [Windows](installation#windows) and [Linux](installation#linux) OSs. Solutions for the most recurring errors are also presented.

## Windows
After downloading the *.exe* file, proceed with each installer step. The executable will automatically request the installation of **Microsoft Visual C ++ Redistributable** and, if it is not installed on your computer, perform the program's procedures.

If the executable fails to open, download the *.zip* file and unzip it to a folder on your system. In that case, it is necessary to manually download and install [**Microsoft Visual C ++ Redistributable 32 bits**](https://aka.ms/vs/16/release/vc_redist.x86.exe).

After the installation is complete, open the PSP-UFU to verify the correct installation. The executable is present in the folder "*<instalation_path>/PSP-UFU/bin/PSP-UFU.exe*".

### Recurring Windows problems
- When downloading the browser you can **falsely** signal that it is a dangerous file, just click on "Keep file";
- When opening the installer, Windows again signals that the file is dangerous, just click "Run anyway";
- When opening the program and changing your language, an error message is displayed. To solve this problem, just run PSP-UFU as an administrator (right click on the program shortcut and then run as administrator). Change the language again and restart the program;
- When creating a new project, the work screen is not displayed or a black screen is displayed. Go to the general options and change the renderer to "Device Context". If an error message appears, run the program as an administrator and repeat the process. Restart the program.


## Linux
Installation on the Linux system is easily accomplished by the * .deb * automatic installation package.
:::caution Attention!
Note that this version for Linux is out of date and some features of the program may not be present.

If you want to contribute to the project and build the current version on the Linux system based on Debian or other distributions, contact us through [GitHub](https://github.com/Thales1330/PSP/issues) or [Twitter](https://twitter.com/PspUfu).
:::

### Recurring Linux problems
It may be necessary to add the [wxWidgets libraries directory](https://wiki.wxwidgets.org/Compiling_and_getting_started#Running_wxWidgets_projects) in the environment variables.
To do this, follow **ONE** of the methods presented below:

#### * Method I (recommended) *
Enter the following command in the **~ / .bashrc** file:
```shell
echo "export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/usr/local/lib" >> ~/.bashrc
```

#### * Method II (not recommended) *
:::warning Watch out!
Using this method, the steps below should be performed ** every time ** that you want to open PSP-UFU.
:::
1. Open the terminal and access the directory where the PSP-UFU is installed:
```shell
cd/usr/local/bin
```
2. Insert the shared libraries of wxWidgets in the environment variables:
```shell
export LD_LIBRARY_PATH = $ LD_LIBRARY_PATH: / usr / local / lib
```
3. Then, run PSP-UFU in the ** same section ** of the terminal:
```shell
./PSP-UFU
```

:::caution Attention!
Check that the computer has the video units installed correctly, otherwise the program will not display the desktop to create and edit single line diagrams or control block diagrams.
:::