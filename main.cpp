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

static void error_callback(int error, const char* description)
{
    std::cerr<<description<<"\n";
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
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

    glfwMakeContextCurrent(window);

    glewExperimental = GL_TRUE;
    glewInit();

    auto mesh1 = StaticMesh::LoadMesh("../resource/cube.obj");
    auto prog = Program::LoadFromFile("../resource/vs.txt", "../resource/fs.txt");
	auto text = Texture2D::LoadFromFile("../resource/brick.png");
	// Remove this line and see the difference
	text.setFilter(FilterMode::eNearestMipmapLinear, FilterMode::eLinear);

	if (!mesh1.hasUV()) {
		std::cerr<<"WARNING: The mesh has no UV data\n";
	}

    auto view = glm::lookAt(glm::vec3{10.0f, 10.0f, 10.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
    auto proj = glm::perspective(glm::pi<float>()/4, 800.0f/600.0f, 0.1f, 100.f);
    auto vp = prog["vp"] = proj*view;
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
        axis.draw(vp);
        l.draw(vp);
        prog.use();
        text.bindToChannel(0);
        mesh1.draw();
        ////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    mesh1.release();
    prog.release();
    text.release();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
