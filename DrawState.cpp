#include "DrawState.h"

static GLenum attrib_array_mapping[] = {GL_FLOAT};

AttribBufferBinding::AttribBufferBinding(uint32_t index, const Buffer &buffer)
    : m_index(index), m_divisor(0), m_bufferid(buffer.id())
{

}

void AttribBufferBinding::divisor(uint32_t div)
{
    m_divisor = div;
}

uint32_t AttribBufferBinding::index() const
{
    return m_index;
}

void ArrayAttrib::bind(const AttribBufferBinding &binding, uint32_t stride, uint32_t offset)
{
    m_binding = binding;
    m_stride = stride;
    m_offset = offset;
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

void DrawState::bind() const
{
    glBindVertexArray(m_vao);
}

void DrawState::release()
{
    glDeleteVertexArrays(1, &m_vao);
}

void DrawState::setArrayAttrib(uint32_t index, const ArrayAttrib &attrib)
{

    glVertexArrayAttribFormat(m_vao, index, attrib.m_size, attrib_array_mapping[static_cast<int>(attrib.m_type)], GL_FALSE, attrib.m_relativeOffset);
    glVertexArrayAttribBinding(m_vao, index, attrib.m_binding.m_index);
    glVertexArrayVertexBuffer(m_vao, index, attrib.m_binding.m_bufferid, attrib.m_offset, attrib.m_stride);
    glVertexArrayBindingDivisor(m_vao, index, attrib.m_binding.m_divisor);
}

void DrawState::enableArrayAttrib(uint32_t index)
{
    glEnableVertexArrayAttrib(m_vao, index);
}

void DrawState::disableArrayAttrib(uint32_t index)
{
    glDisableVertexArrayAttrib(m_vao, index);
}