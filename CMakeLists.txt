cmake_minimum_required(VERSION 2.8)

macro(find_path_of_file ARG_VAR ARG_FILE RELATIVE_PATH DESC)
if(NOT ${ARG_VAR})
file(GLOB_RECURSE ${ARG_VAR}_TMP ${ARG_FILE})
if(${ARG_VAR}_TMP)
get_filename_component(${ARG_VAR}_PATH ${${ARG_VAR}_TMP} DIRECTORY)
get_filename_component(${ARG_VAR} ${${ARG_VAR}_PATH}/${RELATIVE_PATH} ABSOLUTE)
message(STATUS "${DESC} is found in ${${ARG_VAR}}")
else()
message("${DESC} not found, please specify the path or put it inside the ext/ folder.")
set(${ARG_VAR} "" CACHE PATH "")
endif()
endif()
endmacro()


find_path_of_file(GLEW_PATH "${CMAKE_SOURCE_DIR}/ext/glew.pc.in" "./" "GLEW_PATH")
find_path_of_file(GLFW_LIB_PATH "${CMAKE_SOURCE_DIR}/ext/libglfw3.a" "./" "GLFW_LIB_PATH")
find_path_of_file(GLFW_INC_PATH "${CMAKE_SOURCE_DIR}/ext/glfw3.h" "../" "GLFW_INC_PATH")
find_path_of_file(GLM_PATH "${CMAKE_SOURCE_DIR}/ext/glm.hpp" "../" "GLFW_INC_PATH")
set(TINYOBJLOADER_PATH "ext/tinyobjloader")

#STRING(REGEX REPLACE "\\\\" "/" GLEW_PATH ${GLEW_PATH})
#STRING(REGEX REPLACE "\\\\" "/" GLFW_PATH ${GLFW_PATH})

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