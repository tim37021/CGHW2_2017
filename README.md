# cghw2
Basic Rendering with Modern OpenGL

## Build Instruction
This project needs four external libraries, glfw, glm, glew, tinyobjloader. tinyobjloader has been put into /ext directory. The remaining three need to be downloaded and built manually. 

#### Build glm and glew

Simply go to their offical website and download the latest version. Uncompress and put them in /ext.

#### Build glfw
Enter the following command in /ext folder
```bash
git clone https://github.com/glfw/glfw && cd glfw && mkdir build
```
Use cmake to generate project file/makefile.

## Build CGHW2

Make a new folder named build. and use cmake-gui targeting the path of this project. Press configure button. If you put all the dependancies in ext, you are done because cmake will find all the necessary files in ext autometically. Otherwise, GLEW_PATH, GLFW_INC_PATH, GLFW_LIB_PATH need to be specified. GLEW_PATH is the root path of glew. GLFW_INC_PATH is the include path of glfw. GLFW_LIB_PATH is the path where you can find libglfw3.a.
![](/images/cmake_cghw2.png)

Press Generate button and you will see the project file/makefile in build


## Note
You MUST use the same compiler to build glfw and this project.
Currently VC++ is not supported, but this issue will be fixed later.
Please don't copy and paste the commands I list here