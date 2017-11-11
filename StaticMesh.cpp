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

    DrawState &ds = ret.m_drawState;
    ds.bind();

    ret.m_pos = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.positions);
    
    ArrayAttrib attrib;
    attrib.format(3, AttribArrayType::eFloat, 0);
    ds.bindBuffer(0, ret.m_pos, 0, sizeof(GLfloat)*3);
    ds.enableArrayAttrib(0);
    ds.setArrayAttrib(0, attrib, 0);

    if (shapes[0].mesh.texcoords.size() > 0) {
        ret.m_uv = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.texcoords);

        attrib.format(2, AttribArrayType::eFloat, 0);
        ds.bindBuffer(1, ret.m_uv, 0, sizeof(GLfloat)*2);
        ds.enableArrayAttrib(1);
        ds.setArrayAttrib(1, attrib, 1);
    }

    if (shapes[0].mesh.normals.size() > 0) {
        ret.m_normal = ArrayBuffer<GLfloat>::CreateFromSTDVector(ArrayBufferType::eVertex, shapes[0].mesh.normals);

        attrib.format(3, AttribArrayType::eFloat, 0);
        ds.bindBuffer(2, ret.m_normal, 0, sizeof(GLfloat)*3);
        ds.enableArrayAttrib(2);
        ds.setArrayAttrib(2, attrib, 2);
    }

    ret.m_indices = ArrayBuffer<GLuint>::CreateFromSTDVector(ArrayBufferType::eIndex, shapes[0].mesh.indices);
    ds.bindElementBuffer(ret.m_indices);

    glBindVertexArray(0);
    return ret;
}

void StaticMesh::release()
{
    m_drawState.release();
    m_pos.release();
    // free to release empty buffer
    m_uv.release();
    m_normal.release();
    m_indices.release();

}

void StaticMesh::draw()
{
    m_drawState.bind();
    m_drawState.bindElementBuffer(m_indices);
    glDrawElements(GL_TRIANGLES, m_indices.size(), GL_UNSIGNED_INT, nullptr);
}

void StaticMesh::instancedDraw(uint32_t num_inst)
{
    m_drawState.bind();
    m_drawState.bindElementBuffer(m_indices);
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
