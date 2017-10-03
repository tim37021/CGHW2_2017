#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include <GL/glew.h>
#include <string>
class StaticMesh {
public:
    void release();

    static StaticMesh LoadMesh(const std::string &filename);
    void draw();
private:
    GLuint vao;
    GLuint vbo[3];
    GLuint ibo;
    GLuint numIndices;
};

#endif