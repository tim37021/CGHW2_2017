#ifndef _STATIC_MESH_H_
#define _STATIC_MESH_H_

#include <GL/glew.h>
#include <string>
#include "Buffer.h"

class StaticMesh {
public:
    void release();

    static StaticMesh LoadMesh(const std::string &filename);
    void draw();
    void instancedDraw(uint32_t num_inst);

	bool hasNormal() const;
    bool hasUV() const;
    template <class T>
    void setInstanceArray(ArrayBuffer<T> inst_arr);
private:
    StaticMesh();
    GLuint vao;
    ArrayBuffer<GLfloat> m_pos, m_uv, m_normal;
    ArrayBuffer<GLuint> m_indices;
};

#include "StaticMesh.inl"

#endif