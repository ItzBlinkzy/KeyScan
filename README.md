

# KeyScan

<img src="https://github.com/user-attachments/assets/1fbc5331-07b0-401e-840d-4b8264993727" alt="logo" width="125"/>

KeyScan is a simple desktop application for Windows designed for keyboard testing, typing practice, and key remapping.


## Features

* Keyboard Testing Screen: Real-time virtual keyboard display that responds to your physical keyboard inputs (test your keys to see which are working or broken)

* **[WIP]** ~~Typing Test: Play a typing game using the top 1000 English words to practice and improve typing speed.~~

* Key Remapper: Easily remap any keyboard key to another key of your choice.

## Demo
_Demo will be updated as more features are implemented or improved._

https://github.com/user-attachments/assets/2daa6481-a006-426d-989b-c17857d22737

## Installation
**Option 1: Using Pre-built Binaries**

1. Download the latest version from the [releases](https://github.com/ItzBlinkzy/KeyScan/releases) page.
2. Extract the zip file
3. Run KeyScan.exe

**Option 2: Build Locally with Visual Studio**

3. Install Qt VS Tools Extension in Visual Studio
4. Install Qt 6.8.0
    * Open Qt VS Tools menu in Visual Studio
    * Go to Qt Options
    * Add new Qt version and select v6.8.0
    * Set the path to your Qt installation
1. Clone the repository.
    ```
    git clone https://github.com/ItzBlinkzy/KeyScan.git
    ```
6. Open `KeyScan.sln`
7. Build and Run the Solution.

**Option 3: Build locally with CMake and vcpkg**
*Ensure Qt 6.8.0 is installed*
1. **Install vcpkg**:
   If you haven't installed `vcpkg`, follow the instructions [here](https://github.com/microsoft/vcpkg) to set it up.

2. **Install CMake**:
   If you don't have `CMake` installed, download it [here](https://cmake.org/download/).

3. **Clone the repository**:
   ```bash
   git clone https://github.com/ItzBlinkzy/KeyScan.git
   ```
4. In the **root of the repository** run the following commands to build and compile the executable.
    ```bash
    cmake -B build -DCMAKE_TOOLCHAIN_FILE="C:/MYPATHTOVCPKG/vcpkg/scripts/buildsystems/vcpkg.cmake"
    ```

    ```bash
    cmake --build build --config Release
    ```

    ```bash
    ./build/Release/KeyScan.exe
    ```
