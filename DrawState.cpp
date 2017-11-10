#include "DrawState.h"

static GLenum attrib_array_mapping[] = {GL_FLOAT, GL_INT};

ArrayAttrib::ArrayAttrib()
     : m_divisor(0)
{

}

void ArrayAttrib::divisor(uint32_t div)
{
    m_divisor = div;
}

void ArrayAttrib::format(uint32_t size, AttribArrayType type, uint32_t relative_offset)
{
    m_size = size;
    m_type = type;
    m_relativeOffset = relative_offset;
}

DrawState::DrawState()
{
    glGenVertexArrays(1, &m_vao);
}

void DrawState::bindBuffer(uint32_t index, const Buffer &buffer, uint32_t offset, uint32_t stride)
{
    glVertexArrayVertexBuffer(m_vao, index, buffer.id(), offset, stride);
}

void DrawState::bind() const
{
    glBindVertexArray(m_vao);
}

void DrawState::release()
{
    glDeleteVertexArrays(1, &m_vao);
}

void DrawState::setArrayAttrib(uint32_t index, const ArrayAttrib &attrib, uint32_t bindingindex)
{

    glVertexArrayAttribFormat(m_vao, index, attrib.m_size, attrib_array_mapping[static_cast<int>(attrib.m_type)], GL_FALSE, attrib.m_relativeOffset);
    glVertexArrayAttribBinding(m_vao, index, bindingindex);
    glVertexArrayBindingDivisor(m_vao, index, attrib.m_divisor);
}

void DrawState::enableArrayAttrib(uint32_t index)
{
    glEnableVertexArrayAttrib(m_vao, index);
}

void DrawState::disableArrayAttrib(uint32_t index)
{
    glDisableVertexArrayAttrib(m_vao, index);
}