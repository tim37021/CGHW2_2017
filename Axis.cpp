#include "Axis.h"
#include <string>

Axis::Axis(float s)
    : line(), scale(s)
{
    
}

void Axis::draw(const glm::mat4 &vp)
{
    line.set(glm::vec3(0), glm::vec3(1, 0, 0)*scale);
    line.color(glm::vec3(1, 0, 0));
    line.draw(vp);

    line.set(glm::vec3(0), glm::vec3(0, 1, 0)*scale);
    line.color(glm::vec3(0, 1, 0));
    line.draw(vp);

    line.set(glm::vec3(0), glm::vec3(0, 0, 1)*scale);
    line.color(glm::vec3(0, 0, 1));
    line.draw(vp);
}

void Axis::release()
{
    line.release();
}