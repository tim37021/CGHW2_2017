#include "Axis.h"
#include <string>

Axis::Axis(float s)
    : line(), scale(s)
{
    
}

void Axis::draw()
{
    line.set(glm::vec3(0), glm::vec3(1, 0, 0)*scale);
    line.color(glm::vec3(1, 0, 0));
    line.draw();

    line.set(glm::vec3(0), glm::vec3(0, 1, 0)*scale);
    line.color(glm::vec3(0, 1, 0));
    line.draw();

    line.set(glm::vec3(0), glm::vec3(0, 0, 1)*scale);
    line.color(glm::vec3(0, 0, 1));
    line.draw();
}

void Axis::release()
{
    line.release();
}

Program &Axis::getProgram()
{
    return line.getProgram();
}