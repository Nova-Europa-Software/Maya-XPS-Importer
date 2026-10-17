# Maya XPS Importer

An XNALara / XPS importer tool for Autodesk Maya 2025

### This software is designed for Autodesk Maya 2025. It might work with other versions but they are not supported!

## 1. Requirements
Your computer needs to meet following requirements in order to work with this plugin

### Hardware / OS:
- 64-bit CPU using AMD64 architecture
- 10 MB of free disk space
- Windows 10 / Server 2022 64-bit or newer

### Software
- Autodesk Maya 2025

### Additional software needed to build plugin from source
- A C++ compiler with support for C++ 20 standard (MSVC reccomended)
- CMake version 3.19 or newer
- GIT 2.55 or newer
- Autodesk Maya SDK for Autodesk Maya 2025

## 2. Release vs Debug
All the main releases of the plugin contain binaries both for debug
and release build (Debug starting with Dbg_, Release starting with Rel_).

While it is generally recommended to use release build since its more optimized
and stable, using the debug build can be useful when:
- There are problems with loading the plugin
- You encounter an issue while importing a model
- You want more verbose logs
- You want to submit an issue to github repo

## 3. Installing the plugin
To install this plugin on your computer you need to:
- Download the binaries from github
- Put the downloaded .mll file in "C:/Users/%username%/Documents/maya/2025/plug-ins" (If the plug-ins directory does not exist simply create it).
- Start Autodesk Maya
- In the toolbar go to Windows->Settings/Preferences->Plug-in Manager and tick the "Loaded" box next to Rel_MayaXpsImport or Dbg_MayaXpsImport
- (Optional) Also tick the "Auto-load" box so you do not have to repeat the previous step when you want to use the plugin

## 4. Building from source
If you want to build the plugin from source you need to:
- Ensure that you have all the required software installed
- Download the source code from this repo (either as .ZIP archive or using git clone command)
- Go to downloaded project directory and open CMakePresets.json in any generic text editor (Even Windows Notepad will be fine).
- Change MAYASDK_INCLUDE_DIR and MAYASDK_LIBRARY_DIR to where those directories are on your system.
- Generate project files using cmake with the preferred preset
- Compile the project

## 5. Features status
Legned:
- :red_circle: - Unsupported feature
- :orange_circle: - Partialy implemented
- :yellow_circle: - Implemented but untested
- :green_circle: - Tested and implemented feature

Features
- Importing geometry - :green_circle:
- Importing skeleton - :orange_circle:
- Importing textures - :orange_circle:
- Importing materials - :red_circle:
- Importing normals - :green_circle:
- Importing UVs - :red_circle: