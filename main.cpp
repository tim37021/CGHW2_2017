#include <GL/glew.h>
#include <GLFW/glfw3.h>
#include <cstdlib>
#include <cstdio>
#include <string>
#include <tiny_obj_loader.h>
#define GLM_FORCE_RADIANS
#include <glm/glm.hpp>
#include <glm/gtc/matrix_transform.hpp>
#include "ShaderProgram.h"

struct StaticMesh {
    GLuint vao;
    GLuint vbo[3];
    GLuint ibo;
    GLuint numIndices;
    void release();

    static StaticMesh LoadMesh(const std::string &filename);
    void draw();
};

static void error_callback(int error, const char* description)
{
    fputs(description, stderr);
}
static void key_callback(GLFWwindow* window, int key, int scancode, int action, int mods)
{
    if (key == GLFW_KEY_ESCAPE && action == GLFW_PRESS)
        glfwSetWindowShouldClose(window, GL_TRUE);
}

StaticMesh StaticMesh::LoadMesh(const std::string &filename)
{

    std::vector<tinyobj::shape_t> shapes;
	{
		std::vector<tinyobj::material_t> materials;
		std::string error_string;
		if (!tinyobj::LoadObj(shapes, materials, error_string, filename.c_str())) {
			// GG
		}

        /*
		if (shapes.size() == 0)
			GG

		if (shapes[0].mesh.texcoords.size() == 0 || shapes[0].mesh.normals.size() == 0)
			GG*/
	}

    StaticMesh ret;
    glGenVertexArrays(1, &ret.vao);
    glBindVertexArray(ret.vao);

    glGenBuffers(3, ret.vbo);
    glGenBuffers(1, &ret.ibo);

    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
    // Upload postion array
    glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[0]);
    glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.positions.size(),
        shapes[0].mesh.positions.data(), GL_STATIC_DRAW);
    glEnableVertexAttribArray(0);

    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (shapes[0].mesh.texcoords.size() > 0) {

        // Upload texCoord array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[1]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.texcoords.size(),
            shapes[0].mesh.texcoords.data(), GL_STATIC_DRAW);
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
    }

    if (shapes[0].mesh.normals.size() > 0) {
        // Upload normal array
        glBindBuffer(GL_ARRAY_BUFFER, ret.vbo[2]);
        glBufferData(GL_ARRAY_BUFFER, sizeof(GLfloat) * shapes[0].mesh.normals.size(),
            shapes[0].mesh.normals.data(), GL_STATIC_DRAW);

        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
    }

    // Setup index buffer for glDrawElements
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ret.ibo);
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(GLuint) * shapes[0].mesh.indices.size(),
        shapes[0].mesh.indices.data(), GL_STATIC_DRAW);

    ret.numIndices = shapes[0].mesh.indices.size();

    glBindVertexArray(0);
    return ret;
}

void StaticMesh::release()
{
    glDeleteVertexArrays(1, &vao);
    glDeleteBuffers(3, vbo);
    glDeleteBuffers(1, &ibo);

}

void StaticMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, numIndices, GL_UNSIGNED_INT, nullptr);
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

    auto view = glm::lookAt(Vector3f{10.0f, 10.0f, 10.0f}, Vector3f{0.0f, 0.0f, 0.0f}, Vector3f{0.0f, 1.0f, 0.0f});
    auto proj = glm::perspective(3.1415926f/4, 800.0f/600.0f, 0.1f, 100.f);
    prog["vp"] = proj*view;

    glEnable(GL_DEPTH_TEST);
    while (!glfwWindowShouldClose(window))
    {
        // draw
        glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
        auto model = glm::rotate(glm::mat4(1.0f), static_cast<float>(glfwGetTime())
            , glm::vec3(0.0f, 1.0f, 0.0f));
        prog["model"] = model;
        prog.use();
        mesh1.draw();
        ////////////////
        glfwSwapBuffers(window);
        glfwPollEvents();
    }
    mesh1.release();
    glfwDestroyWindow(window);
    glfwTerminate();
    exit(EXIT_SUCCESS);
}
