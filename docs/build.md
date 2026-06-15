# Building the Project

This document explains how to build the project for your platform.  
  
You have 2 ways of building this project: **Release** and **Debug** mode. **Debug** mode includes command line output, used memory and more. **Release** mode removes all of this. 

# Table of Contents
<u>[Requirements](#requirements)</u>  
<u>[Supported Building Platforms](#supported-building-platforms)</u>  
<u>[Setting Up Vulkan](#setting-up-vulkan)</u>  
<u>[Dependencies](#dependencies)</u>  

## Requirements
You need to have the following installed:

- [CMake](https://cmake.org/download/) (Minimum version: **3.25.0**)
- [Clang / LLVM](https://clang.llvm.org/get_started.html)
- [Ninja](https://ninja-build.org/)
- [Vulkan SDK](https://vulkan.lunarg.com/sdk/home)
- [Git](https://git-scm.com/install/)

## Supported Building Platforms

- [Windows](#building-with-windows)
- [Linux](#building-with-linux)
- [MacOS](#building-with-macos)

### Building with Windows   
After you have installed everything simply follow these steps: 

#### Step 1
Clone the Repo by using this command:
```cmd
git clone --recurse-submodules https://github.com/NunoSilva04/Malenia-BossFight.git
```

#### Step 2
List the presets available by running the command:
```cmd
cmake --workflow --list-presets
```
- **Note:** These presets are platform specific. Attempting to use a preset intended for another platform will fail.

Choose the Windows platform and then run the following command for building the project in **Debug** mode:
```cmd
cmake --workflow --preset windows-debug
```

Or run the following command for building the project in **Release** mode:
```cmd
cmake --workflow --preset windows-release
```

#### Step 3 
After the build completes the final executable will be located in: `bin/Windows/<Configuration>`, where `Configuration` is either **Debug** or **Release** depending on the command chosen in [step 2](#step-2). 

### Building with Linux
After you have installed everything simply follow these steps:

#### Step 1
First run the shell script `setup-env.sh` located inside the Vulkan folder.

```cmd
source ~/vulkan/1.x.yy.z/setup-env.sh
``` 

**IMPORTANT:** This command only sets up the environment variables for your current shell or terminal session. Attempting to build the project on another shell or terminal session will result in failure when building the project. If you wish to permantly add them to your please consult your shell or terminal graphical environment documentation.

#### Step 2
List the presets available by running the command:
```cmd
cmake --workflow --list-presets
```
- **Note:** These presets are platform specific. Attempting to use a preset intended for another platform will fail.

Choose the Linux platform and then run the following command for building the project in **Debug** mode:
```cmd
cmake --workflow --preset linux-debug
```

Or run the following command for building the project in **Release** mode:
```cmd
cmake --workflow --preset linux-release
```

#### Step 3 
After the build completes the final executable will be located in: `bin/Linux/<Configuration>`, where `Configuration` is either **Debug** or **Release** depending on the command chosen in [step 2](#step-2-1). 

### Building with MacOS
After you have installed everything simply follow these steps:

#### Step 1
First run the shell script `setup-env.sh` located inside the Vulkan folder.

```cmd
source setup-env.sh
``` 

**IMPORTANT:** This command only sets up the environment variables for your current shell or terminal session. Attempting to build the project on another shell or terminal session will result in failure when building the project. If you wish to permantly add them to your please consult your shell or terminal graphical environment documentation.

#### Step 2
List the presets available by running the command:
```cmd
cmake --workflow --list-presets
```
- **Note:** These presets are platform specific. Attempting to use a preset intended for another platform will fail.

Choose the MacOS platform and then run the following command for building the project in **Debug** mode:
```cmd
cmake --workflow --preset macos-debug
```

Or run the following command for building the project in **Release** mode:
```cmd
cmake --workflow --preset macos-release
```

#### Step 3 
After the build completes the final executable will be located in: `bin/MacOS/<Configuration>`, where `Configuration` is either **Debug** or **Release** depending on the command chosen in [step 2](#step-2-2). 

## Setting Up Vulkan
Here is some extra documentation if you happen to have some trouble setting up Vulkan for your platform.
- [Windows](#setting-up-vulkan-for-windows)
- [Linux](#setting-up-vulkan-for-linux)
- [MacOS](#setting-up-vulkan-for-macos)

### Setting Up Vulkan for Windows
Navigate to [this link](https://vulkan.lunarg.com/sdk/home) and download `vulkansdk-windows-(X64/ARM64)-1.x.yy.z.exe`.  
Run the .exe file by double clicking it, follow the instructions and your set!  

#### Optional Verification (But Important)
Verify that Vulkan has been correctly set up by running one of the following commands: 
```cmd
C:\> vulkaninfoSDK
```

Or:
```cmd
C:\> vkcube
```

### Setting Up Vulkan for Linux
#### Step 1
Navigate to [this link](https://vulkan.lunarg.com/sdk/home) and download `vulkansdk-linux-x86_64-1.x.yy.z.tar.xz`.

#### Step 2
Extract the contents of the folder by running the command: 
```cmd
tar xf path/to/vulkansdk-linux-x86_64-1.x.yy.z.tar.xz
``` 

#### Step 3
Install the runtime dependencies by running the following command:
```cmd  
sudo apt install libxcb-xinput0 libxcb-xinerama0 libxcb-cursor-dev
```
(On Ubuntu for example)  
And you are set!

#### Optional Verification (But Important)
Navigate to the extracted folder (done in [step 2](#step-2-3). The folder should be something named as `1.x.yy.z`) and run the `setup-env.sh` script by running the following commnad:
```cmd
source setup-env.sh
```

Verify that Vulkan has been correctly set up by running one of the following commands: 
```cmd
$ vulkaninfo
```

Or:
```cmd
$ vkcube
```

### Setting Up Vulkan for MacOS
#### Step 1
Navigate to [this link](https://vulkan.lunarg.com/sdk/home) and download `vulkansdk-macos-1.x.yy.z.zip`.  
Double click on the .zip file and run the vulkansdk-macOS-1.v.w.xx.app application.

#### Optional Verification (But Important)
Vulkan will have created a new folder called `vulkansdk` on the default path given or one of your choice. Navigate to this path, search and run the `setup-env.sh` script by running the following commnad:
```cmd
source setup-env.sh
```

Verify that Vulkan has been correctly set up by running the following command: 
```cmd
$ vulkaninfo
```

## Dependencies
Third party libraries used in this project are already included as a submodule and will be automatically compiled and built by CMake.

Submodules dependencies: 
- [SDL3](https://github.com/libsdl-org/SDL)