cmake_minimum_required(VERSION 2.8)

set(GLEW_PATH "" CACHE STRING "GLEW path")
set(GLFW_INC_PATH "" CACHE STRING "GLFW include path")
set(GLFW_LIB_PATH "" CACHE STRING "GLFW lib path")
set(GLM_PATH "" CACHE STRING "GLM path")
set(TINYOBJLOADER_PATH "ext/tinyobjloader")

STRING(REGEX REPLACE "\\\\" "/" GLEW_PATH ${GLEW_PATH})

# ext object loader
include_directories(${GLFW_INC_PATH})
include_directories(${GLM_PATH})
include_directories(${GLEW_PATH}/include)
include_directories(${TINYOBJLOADER_PATH})
include_directories(".")

set(CMAKE_CXX_FLAGS "${CMAKE_CXX_FLAGS} -DGLEW_STATIC -std=c++0x -g")
set(CMAKE_C_FLAGS "${CMAKE_C_FLAGS} -DGLEW_STATIC -std=c99")

project(glew)
set(GLEW_SOURCE ${GLEW_PATH}/src/glew.c)
add_library(glew ${GLEW_SOURCE})

project(tinyobjloader)
set(TOL ${TINYOBJLOADER_PATH}/tiny_obj_loader.cc)
add_library(tinyobjloader ${TOL})

project(cghw2)

link_directories(${GLFW_LIB_PATH})

set(SOURCE_FILES "main.cpp")
set(SOURCE_FILES ${SOURCE_FILES} "ShaderProgram.cpp") 
set(SOURCE_FILES ${SOURCE_FILES} "StaticMesh.cpp") 
add_executable(cghw2 ${SOURCE_FILES})

target_link_libraries(cghw2 glew)
if(WIN32)
target_link_libraries(cghw2 glfw3 gdi32 opengl32)
endif()
if(APPLE)
target_link_libraries(cghw2 glfw3)
target_link_libraries(cghw2 "-framework OpenGL")
target_link_libraries(cghw2 "-framework Cocoa")
target_link_libraries(cghw2 "-framework IOKit")
target_link_libraries(cghw2 "-framework CoreVideo")
endif()
target_link_libraries(cghw2 tinyobjloader)