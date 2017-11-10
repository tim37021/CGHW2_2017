#ifndef _DRAW_STATE_H_
#define _DRAW_STATE_H_

#include <GL/glew.h>
#include <cstdint>
#include "Buffer.h"

enum class AttribArrayType {
    eFloat, eInt
};

class ArrayAttrib;
class AttribBufferBinding;
class DrawState;

class AttribBufferBinding {
    friend class DrawState;
    friend class ArrayAttrib;
public:
    AttribBufferBinding(uint32_t index, const Buffer &buffer);
    void divisor(uint32_t div);
    uint32_t index() const;
private:
    AttribBufferBinding()=default;
    uint32_t m_index;
    uint32_t m_divisor;
    GLuint m_bufferid;
};

class ArrayAttrib {
    friend class DrawState;
public:
    void bind(const AttribBufferBinding &, uint32_t stride, uint32_t offset);
    void format(uint32_t size, AttribArrayType type, uint32_t relative_offset);
private:
    //// save paramaters...
    uint32_t m_size, m_relativeOffset;
    AttribArrayType m_type;

    AttribBufferBinding m_binding;
    uint32_t m_stride, m_offset;
};

class DrawState {
public:
    DrawState();
    void enableArrayAttrib(uint32_t index);
    void disableArrayAttrib(uint32_t index);
    void setArrayAttrib(uint32_t index, const ArrayAttrib &);
    void bind() const;
    void release();
private:
    GLuint m_vao;
};

#endif