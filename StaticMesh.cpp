#include "StaticMesh.h"
#include <tiny_obj_loader.h>

StaticMesh::StaticMesh()
    : m_hasUV(false), m_hasNormal(false)
{

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

    ret.m_pos = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.positions);
    ret.m_pos.bind();
    glEnableVertexAttribArray(0);
    glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 0, 0);
    
    if (shapes[0].mesh.texcoords.size() > 0) {
        ret.m_uv = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.texcoords);
        ret.m_uv.bind();
        glEnableVertexAttribArray(1);
        glVertexAttribPointer(1, 2, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_hasUV = true;
    }

    if (shapes[0].mesh.normals.size() > 0) {
        ret.m_normal = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.normals);
        ret.m_normal.bind();
        glEnableVertexAttribArray(2);
        glVertexAttribPointer(2, 3, GL_FLOAT, GL_FALSE, 0, 0);
		ret.m_hasNormal = true;
    }

    ret.m_indices = ArrayBuffer<GLuint>::CreateFromSTDVector(ArrayBufferType::eIndex, shapes[0].mesh.indices);

    glBindVertexArray(0);
    return ret;
}

void StaticMesh::release()
{
    glDeleteVertexArrays(1, &vao);
    m_pos.release();
    // free to release empty buffer
    m_uv.release();
    m_normal.release();
    m_indices.release();

}

void StaticMesh::draw()
{
    glBindVertexArray(vao);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

bool StaticMesh::hasNormal() const
{
	return m_hasNormal;
}

bool StaticMesh::hasUV() const 
{
	return m_hasUV;
}
