#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <iostream>
#include <string>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"
#include "StaticMesh.h"
#include "Texture.h"
#include "Axis.h"
#include "Camera.h"

int calcFlat = 0;

static void error_callback(int error, const char* description)
{
    std::cerr<<description<<"\n";
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);

    if (key == GLFW_KEY_F && action == GLFW_PRESS)
        calcFlat = !calcFlat;

}


static void updateCamera(GLFWwindow *window, Camera &cam) 
{
    static double last_time;
    static double last_x, last_y;
    static bool hasInitialized = false;
    if(!hasInitialized) {
        last_time = glfwGetTime();
        glfwGetCursorPos(window, &last_x, &last_y);
        hasInitialized = true;
    }

    CameraMovement cm = CameraMovement::eStable;
    if(glfwGetKey(window, GLFW_KEY_W) == GLFW_PRESS) 
        cm = cm | CameraMovement::eForward;
    if(glfwGetKey(window, GLFW_KEY_S) == GLFW_PRESS) 
        cm = cm | CameraMovement::eBackward;
    if(glfwGetKey(window, GLFW_KEY_A) == GLFW_PRESS) 
        cm = cm | CameraMovement::eLeft;
    if(glfwGetKey(window, GLFW_KEY_D) == GLFW_PRESS) 
        cm = cm | CameraMovement::eRight;
    double x, y;
    glfwGetCursorPos(window, &x, &y);
    
    cam.processKeyboard(cm, static_cast<float>(glfwGetTime()-last_time));
    cam.processMouseMovement(static_cast<float>(x-last_x), static_cast<float>(last_y-y));

    last_x = x;
    last_y = y;
    last_time = glfwGetTime();
}

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // Mac OS X is not allow to use >= 3.3
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 3);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 3);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // for Mac OS X
    glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);
    glfwSetInputMode(window, GLFW_CURSOR, GLFW_CURSOR_DISABLED);

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    auto mesh1 = StaticMesh::LoadMesh("../resource/sphere.obj");
    auto prog = Program::LoadFromFile("../resource/vs.txt", "../resource/gs.txt", "../resource/fs.txt");
	auto text = Texture2D::LoadFromFile("../resource/brick.png");
	// Remove this line and see the difference
	text.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh1.hasUV()) {
		std::cerr<<"WARNING: The mesh has no UV data\n";
	}

    Camera cam;
    auto proj = glm::perspective(glm::pi<float>()/4, 800.0f/600.0f, 0.1f, 100.f);
    prog["text"] = 0;
    auto uniform_model = prog["model"];


    Axis axis(7.0f);
    glEnable(GL_DEPTH_TEST);

    Line l;
    l.set(glm::vec3(1, 5, 0), glm::vec3(5, 5, 5));
    while (!glfwWindowShouldClose(window))
    {
        // draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto model = glm::rotate(glm::mat4(1.0f), static_cast<float>(glfwGetTime())
            , glm::vec3(0.0f, 1.0f, 0.0f));
        uniform_model = model;

        glm::mat4 vp = proj * cam.getViewMatrix();

        axis.draw(vp);
        l.draw(vp);
        prog.use();
        prog["vp"] = vp;
        prog["calcFlatNormal"] = calcFlat;
        text.bindToChannel(0);
        mesh1.draw();
        ////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
        updateCamera(window, cam);
    }
    mesh1.release();
    prog.release();
    text.release();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
