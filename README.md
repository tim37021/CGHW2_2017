# cghw2
Basic Rendering with Modern OpenGL

## Build Instruction
This project needs five external libraries, glfw, glm, glew, tinyobjloader, lodepng. tinyobjloader and lodepng have been put into /ext directory. The remaining three need to be downloaded and built manually, since they are updated frequently. For following instruction, you will need cmake and git. Please install them before continuing.



#### Step 1: Download External Libraries

Simply go to their offical website and download the latest version. 

* glew (Uncompress and put in /ext)
    http://glew.sourceforge.net/
* glm (Uncompress and put in /ext)
    https://github.com/g-truc/glm/tags
* glfw (Enter the following command in /ext)
    ```bash
    git clone https://github.com/glfw/glfw
    ```
   (Linux Only!!!)Install Package for GLFW
   ```bash
   sudo apt-get install xorg-dev
   ```

#### Step 2: CMake
1. At root directory of this repo. Make a new folder which has name start with "build". For example build_mingw or build_msvc
    ```bash
    # Don't copy me please...
    mkdir build_mingw
   ```
   
2. Open cmake-gui and follow the steps in the figure.

![](/images/cmake.png)

This dialog wants you to choose an IDE(Compiler/Build Tool). You may want to choose "Unix Makefiles" in Linux-like or MacOSX. I normally use mingw-gcc and Visual Studio, so my selections are "MinGW Makefiles" and "Visual Studio 14 2015 Win64"

![](/images/generator.png)

#### Step 3: Build & Run

After you press generate button, there will be a project file/makefile in the build folder. You can use IDE to open it(*.sln file) or use "make" to build.
The following are the corressponding build commands for different makefiles generators. If you use IDE, you can skip this.
* Unix Makefiles
    ```bash
    make
    ```
* MinGW Makefiles
    ```bash
    mingw32-make
    ```

## Note
* If you want to add additional cpp files, simply re-cmake in build folder. It will adject Makefiles/Projectfiles autometically.
* Well support for all platforms (Mac OS X, Linux, Windows) and IDEs/Compilers (xcode, codeblocks, visual studio). 
* Please don't copy and paste the commands I list here. Try to understand it.