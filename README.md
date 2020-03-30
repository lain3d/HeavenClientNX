# HeavenClient
HeavenClient is a custom, made-from-scratch game client.

# Switch Version

## Building
- Ensure you have setup DevkitPro by following the guide [here](https://switchbrew.org/wiki/Setting_up_Development_Environment)
- Ensure your environment variables are setup by executing ```source $DEVKITPRO/switchvars.sh``` in your shell. $DEVKITPRO is usually located at ```/opt/devkitpro```.
- Ensure you have all devkitpro dependencies (see below)
- Run ```./build-deps.sh``` then ```./build-switch.sh```

## Setting up your SD card
- Move the resulting .nro from the switch-build directory to the ```switch``` directory of your SD card.
- Copy all the .nx files (excluding MapPretty.nx and MapLatest.nx), the fonts folder, and the Settings file to the ```switch/HeavenClient``` directory. These nx files are the same ones you would have to have for the pc version [here](https://github.com/ryantpayton/HeavenClient). You need to use a wz->nx converter you can get [here](https://drive.google.com/file/d/1Mk3Kq1lY4NTMqylN5sn0-DQNAcoZZRYH/view?usp=sharing). These wz files MUST be clean, with no wz edits. (plzfix)

## Setting up configuration
- Configure your IP address, account, password, and controls (keyboard mappings to switch controller) all in the Settings file that you put in your ```/switch/HeavenClient``` directory. Note that you can use special keys like left ctrl, alt, etc if you put the 'codes' from glfw3 instead of letters. See [here.](https://github.com/lain3d/glfw/blob/4373905b73468c3f884f3a2560b592521a5a923e/include/GLFW/glfw3.h#L361)

## Quick changes / debugging 
If for whatever reason the client doesn't start, you will probably want to figure out why. I've added debug prints in the majority of the startup phase. Also, the following method will let you update the client without moving it to your sd card each time.
- Go to the nx-link tool located at ```/opt/devkitpro/tools/bin```
- Run ./nxlink -a <switch_ip> -s -p ./HeavenClient.nro <location of your .nro>

### Dependencies from DevkitPro 
Not including the dependencies found in ```libs/``` you should check ```CMakeLists.txt``` and install any dependencies using pacman. Pacman will be used even if you are on a linux distro other than Arch, you may just need to use dkp-pacman instead.

### Rebuilding
- Don't waste your time running ```./build-switch.sh``` every time, as it deletes the build directory; cd into switch-build and run make -jN (where N is number of cores)

### Help
- If you have any issues setting this up, feel free to message me on Discord at surge#1111

# Supported versions
The client is currently compatible with version 83 servers.
The client has only been tested with [HeavenMS](https://github.com/ronancpl/HeavenMS).

# Configuration
The build can be configured by editing the **MapleStory.h** file. The following options are available:
- **USE_CRYPTO**: Use cryptography when communicating for the server.
- **PRINT_WARNINGS**: Print warnings and minor errors to the console.

The default settings can be configured by editing the **Configuration.h** file. These are also generated after a game session in a file called **Settings**. These can be altered in the same way as **Configuration.h**, although, these do not persist if you delete the file, unlike **Configuration.h**.

# Required Files
*Always check **NxFiles.h** for an updated list of required nx files*
- Map001.nx (Latest GMS Map001.wz)
- UI.nx (Latest GMS UI.wz)
- Everything else is from v83 GMS wz files

# Donations
If you feel obligated to donate, to further help and support all parties involved in the development of the HeavenClient project, you can donate using [this](https://paypal.me/pools/c/8frYNoobcY) link.

Please remember this is ONLY for the HeavenClient development and will only be used in the support of helping further develop the client. *Also please remember to support Nexon as this is not meant to replace anything Nexon offers*

Another important note to remember is that HeavenClient is a free open-sourced client developed for personal use. Do NOT pay for any services requested by anyone in regards to this client. It will always remain open and free of charge. There is no intent to publish this code with any payment in mind. If that ever changes, donations and disclaimers for donations will be removed.

# Credits
- @ryantpayton - for the development of the continued Journey client (HeavenClient)
- @fgsfdsfgs - For the OpenAL port and answering questions glfw3 and glad
- @exelix - advice on using nxlink to get stdout 
