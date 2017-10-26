#include "Line.h"
#include <string>

Line::Line() 
    : prog(Program::LoadFromFile("../resource/vs_line.txt", "../resource/fs_line.txt"))
{
    glGenVertexArrays(1, &vao);
    uvp = prog["vp"];
    ua = prog["data[0]"];
    ub = prog["data[1]"];
    ucolor = prog["lColor"];
}

void Line::set(const glm::vec3 &a, const glm::vec3 &b)
{
    ua = a;
    ub = b;
}

void Line::draw(const glm::mat4 &vp)
{
    uvp = vp;


    glBindVertexArray(vao);
    prog.use();
    glDrawArrays(GL_LINES, 0, 2);
}

void Line::release()
{
    glDeleteVertexArrays(1, &vao);
    prog.release();
}

void Line::color(const glm::vec3 &color)
{
    ucolor = color;
}