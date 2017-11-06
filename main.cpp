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
#include "Buffer.h"

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

int main(void)
{
    glfwSetErrorCallback(error_callback);
    if (!glfwInit())
        exit(EXIT_FAILURE);

    // drop macos support
    glfwWindowHint(GLFW_CONTEXT_VERSION_MAJOR, 4);
    glfwWindowHint(GLFW_CONTEXT_VERSION_MINOR, 5);
    glfwWindowHint(GLFW_OPENGL_PROFILE, GLFW_OPENGL_CORE_PROFILE);
    // Bye bye mac os X. We are using 4.5
    //glfwWindowHint(GLFW_OPENGL_FORWARD_COMPAT, GL_TRUE);

    GLFWwindow* window = glfwCreateWindow(800, 600, "Simple example", NULL, NULL);
    glfwSetKeyCallback(window, key_callback);

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
    // upload mesh instance data
    std::vector<glm::vec3> offset;
    for(int i=0; i<10; i++) {
        for(int j=0; j<10; j++) {
            offset.push_back({-5+i, 0, -5+j});
        }
    }
    auto inst = ArrayBuffer<GLfloat>(ArrayBufferType::eVertex);
    inst.allocate(AccessLevel::eDeviceLocal, offset.size()*sizeof(glm::vec3), offset.data());
    mesh1.setInstanceArray(inst);

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
        prog["calcFlatNormal"] = calcFlat;
        text.bindToChannel(0);
        mesh1.instancedDraw(offset.size());
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
