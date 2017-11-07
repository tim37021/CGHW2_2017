#ifndef _AXIS_H_
#define _AXIS_H_

#include <GL/glew.h>
#include <glm/glm.hpp>
#include "Line.h"

class Axis {
public:
    Axis(float s=1.0f);
    void draw();
    void release();
    Program &getProgram();
private:
    Line line;
    float scale;
};

#endif