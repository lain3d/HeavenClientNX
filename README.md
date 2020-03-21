# HeavenClient
HeavenClient is a custom, made-from-scratch game client.

# Switch Version

## Building
- Ensure you have setup DevkitPro by following the guide [here](https://switchbrew.org/wiki/Setting_up_Development_Environment)
- Ensure you have all devkitpro dependencies (see below)
- Run ```./build-deps.sh``` then ```./build-switch.sh```

## Setting up your SD card
- Move the resulting .nro from the switch-build directory to the ```switch``` directory of your SD card.
- Copy all the .nx files (excluding MapPretty.nx and MapLatest.nx), the fonts folder, and the Settings file to the ```switch/HeavenClient``` directory

## Quick changes / debugging 
If for whatever reason the client doesn't start, you will probably want to figure out why. I've added debug prints in the majority of the startup phase. Also, the following method will let you update the client without moving it to your sd card each time.
- Go to the nx-link tool located at ```/opt/devkitpro/tools/bin```
- Run ./nxlink -a <switch_ip> -s -p ./HeavenClient.nro <location of your .nro>

### Dependencies from DevkitPro 
Not including the dependencies found in ```libs/``` you should check ```CMakeLists.txt``` and install any dependencies using pacman.

### Rebuilding
- Don't waste your time running ```./build-switch.sh``` every time, as it deletes the build directory; cd into switch-build and run make -jN (where N is number of cores)

### Help
- If you have any issues setting this up, feel free to message me on Discord at surge#1111

# Supported versions
The client is currently compatible with version 83 servers.
The client has only been tested with [HeavenMS](https://github.com/ronancpl/HeavenMS).

# Configuration
The build can be configured by editing the **MapleStory.h** file. The following options are available:
- **USE_ASIO**: Use Asio for networking (additional dependency)
- **USE_XXHASH** : Use xxhash for file check (additional dependency)
- **USE_CRYPTO**: Use cryptography when communicating for the server.
- **PRINT_WARNINGS**: Print warnings and minor errors to the console.

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

# Building
1. Open **MapleStory.sln** in Visual Studio 2017 CE
2. Make sure to use **Windows SDK Version: 8.1** and **Platform Toolset: v140** (If you don't have these, download them)
   * [Windows 8.1 SDK](https://developer.microsoft.com/en-us/windows/downloads/sdk-archive)
3. Press **Build** > **Build Solution** or **Ctrl + Shift + B**
4. After a successful build, you can now run the program by pressing **Debug** > **Start Debugging** or **F5**
5. Make sure all nx files are present in the parent folder. To convert wz files to nx you can use [NoLifeStory.zip](https://drive.google.com/open?id=1Mk3Kq1lY4NTMqylN5sn0-DQNAcoZZRYH)
   - Extract the zip
   - Place your wz files in the **files** folder
   - Run **start.bat**
   - See **Required Files** for a list of required nx files

# Required Files
*Always check **NxFiles.h** for an updated list of required nx files*
- MapPretty.nx (v167 GMS Map.wz)
- MapLatest.nx (Lastest GMS Map.wz)
- Map001.nx (Latest GMS Map001.wz)
- UI.nx (Latest GMS UI.wz)
- Everything else is from v83 GMS wz files

# Dependencies
- Nx library:
[NoLifeNX](https://github.com/NoLifeDev/NoLifeNx)

- Graphics:
[GLFW3](http://www.glfw.org/download.html), [GLEW](http://glew.sourceforge.net/), [FreeType](http://www.freetype.org/)

- Audio:
[Bass](http://www.un4seen.com/)

- Utility:
[xxHash](https://github.com/Cyan4973/xxHash) (optional)

- Networking:
[Asio](http://think-async.com/) (optional)

# In-Game Issues
If you experience any kind of in-game glitches, UI rendering issues, or anything else that seems out of the ordinary that other developers are not experiences; Follow these steps in order to hopefully resolve aforementioned issues.
1. Clean Solution
2. Close Visual Studio
3. Delete the following files/folders: **.vs**, **x64**, **debug.log**, **MapleStory.aps**, **Settings**
4. Open Solution
5. Rebuild Solution

# Binaries (08.19.2019)
The latest build ([ddd67c8](https://github.com/ryantpayton/HeavenClient/commit/ddd67c8693ef2ad352e8c2dc1e4628e625c49953)) can be found here: [HeavenClient.zip](https://drive.google.com/open?id=186fBZWcuK5uSB3CN6jeTJ_AdU6tK82Ey)

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this](https://paypal.me/pools/c/8frYNoobcY) link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.
