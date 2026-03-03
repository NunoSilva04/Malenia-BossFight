# Malenia-BossFight

## Disclaimer
    - **  **

## Build The Project

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

Then choose the preset for your Operating System, (in this example, windows in debug version), and run the command:
```cmd
cmake --workflow windows-debug
```
or this command, in case your cmake version is not up to date:
```cmd
cmake --workflow --preset windows-debug
```