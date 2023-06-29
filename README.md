# NotGenshin Clock (v0.5.0)
A Genshin Impact style of clock face created with [arm-2d](https://github.com/ARM-software/Arm-2D)

![ClockFace](./doc/pictures/clock_face.png) 

### Disclaimer:

Apart from the background image, which is derived from a game screenshot, this open-source project does not directly utilize any resource files from Genshin Impact. All materials necessary for the clock face were created by myself, yet unavoidably reference the original design of Genshin Impact. The copyright for these designs belongs to the original game company. 

The image materials involved in this open-source project, that is, the converted pixel array source code files (stored in the `genshin_clock/resources` directory), are **NOT** to be used for any commercial purposes and are solely for open-source and learning purposes. This rule supersedes the Apache 2.0 License that this project uses. 

The rest of the source code in this project adheres to the Apache 2.0 License.



## How to Build



### 1. Clone the Repository

First, you need to clone the project to your local machine. You can do this by running the following command in your terminal:

```bash
git clone --recursive https://github.com/GorgonMeducer/GenshinClock.git
```

The `--recursive` option is used to clone all the submodules included in the project.

### 2. Install LLVM

Next, you need to download and install LLVM. You **MUST** download the [latest win32 version of LLVM](https://github.com/llvm/llvm-project/releases).

**NOTE**: During the installation of LLVM, make sure you choose the option to add LLVM to your PATH. After installation, you may need to restart your computer to ensure the new environment variable settings take effect.

### 3. Open the Project

Finally, you need to open the project in VS Code. You can do this by choosing "Open Folder" from the File menu in VS Code, and then selecting the folder of the `GenshinClock` project that you cloned.

Now, you should be ready to start using the GenshinClock project. If you encounter any issues, feel free to ask for help.



Enjoy.
