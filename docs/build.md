# Building the Project

You have 2 ways of building this project: **Release** and **Debug** mode. **Debug** mode includes command line output, used memory and more. **Release** mode removes all of this. 

## Requirements
You need to have the following installed:

- [C/C++]()
- [CMake]() 
- [Clang]()
- [Vulkan]()
- [Git]()

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

### Step 3 

After the build completes the final executable will be located in: `bin/<Platform>/<Configuration>`

For this example:
```cmd
C:\...\Malenia-BossFight\bin\Windows\Debug
``` 