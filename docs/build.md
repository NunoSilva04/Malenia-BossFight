# Building the Project

This document explains how to build the project for your platform.  
  
You have 2 ways of building this project: **Release** and **Debug** mode. **Debug** mode includes command line output, used memory and more. **Release** mode removes all of this. 

## Supported Platforms

- Windows
- MacOS
- Linux

## Requirements
You need to have the following installed:

- [CMake](https://cmake.org/download/) (Minimum version: **3.25.0**)
- [Clang / LLVM](https://clang.llvm.org/get_started.html)
- [Ninja](https://ninja-build.org/)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [Git](https://git-scm.com/install/)

## Process
After you have installed everything simply follow these steps: 

### Step 1

Clone the Repo by using this command:
```cmd
git clone --recurse-submodules https://github.com/NunoSilva04/Malenia-BossFight.git
```

### Step 2

List the presets available by running the command:
```cmd
cmake --workflow --list-presets
```

Then choose the preset for your platform and what type of configuration you would like and run the command:
```cmd
cmake --workflow --preset <Platform-Configuration>
```

Example (Platform - Windows | Configuration - Debug): 
```cmd
cmake --workflow --preset windows-debug
```

- **Note:** These presets are platform specific. Attempting to use a preset intended for another platform will fail.

### Step 3 

After the build completes the final executable will be located in: `bin/<Platform>/<Configuration>`

For this example:
```cmd
C:\...\Malenia-BossFight\bin\Windows\Debug
``` 

## Dependencies

Third party libraries used in this project are already included as a submodule and will be automatically compiled and built by CMake.

Submodules dependencies: 
- [SDL3](https://github.com/libsdl-org/SDL)