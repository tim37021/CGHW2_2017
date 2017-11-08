#include "Buffer.h"
#include <glm/glm.hpp>

Buffer::Buffer()
    : m_ptr(nullptr), m_id(0)
{
    glCreateBuffers(1, &m_id);
}

Buffer::Buffer(GLuint id)
    : m_ptr(nullptr), m_id(id)
{

}

void Buffer::release()
{
    glDeleteBuffers(1, &m_id);
}


bool Buffer::valid() const
{
    return glIsBuffer(m_id);
}

void *Buffer::map(AccessLevel level, uint32_t offset, uint32_t size) 
{
    GLbitfield flags = 0;
    flags |= static_cast<int>(level&AccessLevel::eRead) ? GL_MAP_READ_BIT: 0;
    flags |= static_cast<int>(level&AccessLevel::eWrite) ? GL_MAP_WRITE_BIT: 0;
    flags |= static_cast<int>(level&AccessLevel::ePersistent) ? GL_MAP_PERSISTENT_BIT: 0;
    flags |= static_cast<int>(level&AccessLevel::eCoherent) ? GL_MAP_COHERENT_BIT: 0;
    size = glm::clamp<uint32_t>(size, 0, m_size-offset);
    if(size>0) {
        m_ptr = glMapNamedBufferRange(m_id, offset, size, flags);
    }
    
    return m_ptr;
}

void Buffer::unmap()
{
    if(m_ptr)
        glUnmapNamedBuffer(m_id);
    m_ptr = nullptr;
}

void Buffer::allocate(AccessLevel level, uint32_t size, const void *data)
{
    unmap();

    m_size = size;

    GLbitfield flags = 0;
    flags |= static_cast<int>(level&AccessLevel::eRead) ? GL_MAP_READ_BIT: 0;
    flags |= static_cast<int>(level&AccessLevel::eWrite) ? GL_MAP_WRITE_BIT: 0;
    flags |= static_cast<int>(level&AccessLevel::ePersistent) ? GL_MAP_PERSISTENT_BIT: 0;
    flags |= static_cast<int>(level&AccessLevel::eCoherent) ? GL_MAP_COHERENT_BIT: 0;

    //bind();
    glNamedBufferStorage(m_id, size, data, flags);
    GLenum err = glGetError();
    err = 1;
}

void Buffer::bind(GLenum target) const
{
    glBindBuffer(target, m_id);
}

GLuint Buffer::id() const
{
    return m_id;
}
