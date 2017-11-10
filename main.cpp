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
#include "Camera.h"

int calcFlat = 0;
bool enableCamera = true;

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
    if (key == GLFW_KEY_C && action == GLFW_PRESS) {
        enableCamera = !enableCamera;
        glfwSetInputMode(window, GLFW_CURSOR, enableCamera? GLFW_CURSOR_DISABLED: GLFW_CURSOR_NORMAL);
    }
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
#include <algorithm>

struct VertexData {
    glm::vec3 offset;
    int show;
};

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

    auto inst = ArrayBuffer<VertexData>(ArrayBufferType::eVertex);
    inst.allocateElements(AccessLevel::eDeviceLocal, 1000);
    mesh1.setInstanceArray(inst, 3, 0);

    // create shared buffer
    auto inst_ssbo = ShaderStorage<VertexData>(inst);
    auto prog_cs = Program::LoadFromFile("../resource/cs.txt");
    //inst_ssbo.bind();
    prog_cs.bindShaderStorage("UserData", 0, inst_ssbo);

    auto updateBuffer = [prog_cs, inst_ssbo] (float t) {
        prog_cs["time"] = t;
        prog_cs.dispatchCompute(10, 10, 10);
        glMemoryBarrier(GL_SHADER_STORAGE_BARRIER_BIT);
    };
    

    updateBuffer(0.f);
    inst.bind();

    auto vp_buffer = UniformBuffer<glm::mat4>();
    vp_buffer.allocate(AccessLevel::eWrite | AccessLevel::eCoherent | AccessLevel::ePersistent);
    glm::mat4 &vp = *vp_buffer.mapStructure(AccessLevel::eWrite | AccessLevel::eCoherent | AccessLevel::ePersistent);
    
    Camera cam({10.0f, 0.f, 0.f});
    auto view = glm::lookAt(glm::vec3{10.0f, 10.0f, 10.0f}, glm::vec3{0.0f, 0.0f, 0.0f}, glm::vec3{0.0f, 1.0f, 0.0f});
    auto proj = glm::perspective(glm::pi<float>()/4, 800.0f/600.0f, 0.1f, 100.f);
    prog["text"] = 0;
    prog.bindUniformBuffer("UserData", 0, vp_buffer);
    auto uniform_model = prog["model"];


    Axis axis(7.0f);
    Line l;
    l.set(glm::vec3(1, 5, 0), glm::vec3(5, 5, 5));

    l.getProgram().bindUniformBuffer("UserData", 0, vp_buffer);
    axis.getProgram().bindUniformBuffer("UserData", 0, vp_buffer);

    glEnable(GL_DEPTH_TEST);
    glEnable(GL_CULL_FACE);

    while (!glfwWindowShouldClose(window))
    {
        // draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto model = glm::rotate(glm::mat4(1.0f), static_cast<float>(1.0f)
            , glm::vec3(0.0f, 1.0f, 0.0f)) * glm::scale(glm::mat4(1.0f), glm::vec3(0.2f));
        uniform_model = model;

        vp = proj*cam.getViewMatrix();

        axis.draw();
        l.draw();
        updateBuffer(glfwGetTime());
        //
        prog.use();
        prog["calcFlatNormal"] = calcFlat;
        text.bindToChannel(0);
        mesh1.instancedDraw(1000);
        ////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
        if(enableCamera)
            updateCamera(window, cam);
    }
    mesh1.release();

    vp_buffer.unmap();
    vp_buffer.release();
    inst.release();
    prog.release();
    prog_cs.release();
    text.release();
    axis.release();
    l.release();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
