# Malenia-BossFight 
## PreRequisites

In order to build this project you need to have installed

 - [git](https://git-scm.com/downloads/win)
 - [vcpkg](https://learn.microsoft.com/en-us/vcpkg/get_started/get-started?pivots=shell-powershell) 
 - [cmake](https://cmake.org/download/)

## Build The Project

### Step 1 
Open a command prompt and navigate to the directory where you want to have the project. Once there run the command

`git clone https://github.com/NunoSilva04/Malenia-BossFight.git`

### Step 2
Download the third party assets zip folder from the git repository [release page](https://github.com/NunoSilva04/Malenia-BossFight/releases/tag/v1.0)


### Step 3
Extract the zip anywhere you like and copy the file path.

Example:
```
C:/example/path/to/third-party-assets 
```

**Make sure you replace all instances of `\` for `/` in your file path**

**IMPORTANT:**  
The project expects all third-party assets to be inside a folder called `third-party-assets`.  

- If extracting the zip creates a bunch of files directly on your desktop (or other location) instead of a folder, **manually create a folder named `third-party-assets`** and move all extracted contents into it. 
- In the end you should have something like this 

![extracted_folders](ReadMe_Pictures/extracted_folders.png)

- **Don't forget to copy the file path and replace all instances of `\` for `/`**

### Step 4

Open the project and go to **third-party-assets/portfile.cmake** and replace all instance of **pathToAssets** with your new file path

### Step 5

Open a command prompt and navigate all the way until the **vcpkg** folder 

Example:
```cmd
C:\vcpkg>
```

### Step 6

Run the command `vcpkg install third-party-assets --overlay-ports=C:\example\path\to\third-party-assets`

**IMPORTANT:**
Note that here we are using `\` instead of `/`. 

After the command executes you should see inside your **vcpkg folder** a new folder called **packages** and inside it another folder called
**third-party-assets_x64-windows**. Enter that folder and copy that file path.

Example: **"C:/.../vcpkg/packages/third-party-assets_x64-windows"**

**IMPORTANT:**
Note that here we are now using `/` instead of `\`. 

### Step 7

Go back to the project folder and go to **CMakeLists.txt**. 
Find the command 
```cmake
set(THIRD_PARTY_ASSETS_DIR "C:/vcpkg/packages/third-party-assets_x64-windows") 
```

and replace **"C:/vcpkg/..."** with the file path that you had previously copied in step 6

### Step 8

Create a directory (for example build) in the main project folder. After that run the command `cmake -B <directory_name>`

Your folder structure should look something like this

![project_structure](ReadMe_Pictures/project_structure.png)

### Step 9

Once finished run the command `cmake --build <directory_name>`

### Step 10

The .exe will be located inside the **<directory_name>/Debug folder**. For that just navigate all the way until where the **.exe** is located and run the command `bossFight.exe`

# Disclaimer

Make sure you have either a PS4 or a PS5 controller