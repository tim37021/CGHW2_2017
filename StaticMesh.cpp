#include "StaticMesh.h"
#include <tiny_obj_loader.h>

StaticMesh::StaticMesh()
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
    glVertexAttribFormat(0, 3, GL_FLOAT, GL_FALSE, 0);
    glVertexAttribBinding(0, 0);
    glBindVertexBuffer(0, ret.m_pos.id(), 0, sizeof(float)*3);

    if (shapes[0].mesh.texcoords.size() > 0) {
        ret.m_uv = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.texcoords);

        glEnableVertexAttribArray(1);
        glVertexAttribFormat(1, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(1, 1);
        glBindVertexBuffer(1, ret.m_uv.id(), 0, sizeof(float)*2);
    }

    if (shapes[0].mesh.normals.size() > 0) {
        ret.m_normal = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.normals);

        glEnableVertexAttribArray(2);
        glVertexAttribFormat(2, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(2, 2);
        glBindVertexBuffer(2, ret.m_normal.id(), 0, sizeof(float)*3);
    }

    ret.m_indices = ArrayBuffer<GLuint>::CreateFromSTDVector(ArrayBufferType::eIndex, shapes[0].mesh.indices);
    ret.m_indices.bind();

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

void StaticMesh::instancedDraw(uint32_t num_inst)
{
    glBindVertexArray(vao);
    glDrawElementsInstanced(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr, num_inst);
}

bool StaticMesh::hasNormal() const
{
	return m_normal.valid();
}

bool StaticMesh::hasUV() const 
{
	return m_uv.valid();
}
