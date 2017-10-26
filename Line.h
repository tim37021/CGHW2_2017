#ifndef _LINE_H_
#define _LINE_H_

#include <GL/glew.h>
#include "ShaderProgram.h"

class Line {
public:
    Line();
    void set(const glm::vec3 &a, const glm::vec3 &b);
    void color(const glm::vec3 &color);
    void draw(const glm::mat4 &vp);
    void release();
private:
    GLuint vao;
    Program prog;
    UniformVariable uvp, ua, ub, ucolor;
};

#endif