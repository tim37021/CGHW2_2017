#ifndef _DRAW_STATE_H_
#define _DRAW_STATE_H_

#include <GL/glew.h>
#include <cstdint>
#include "Buffer.h"

enum class AttribArrayType {
    eFloat, eInt
};

class ArrayAttrib;
class DrawState;


class ArrayAttrib {
    friend class DrawState;
public:
    ArrayAttrib();
    void format(uint32_t size, AttribArrayType type, uint32_t relative_offset);
    void divisor(uint32_t div);
private:
    //// save paramaters...
    uint32_t m_size, m_relativeOffset;
    AttribArrayType m_type;
    uint32_t m_divisor;
};

class DrawState {
public:
    DrawState();
    void bindBuffer(uint32_t index, const Buffer &buffer, uint32_t offset, uint32_t stride);
    void enableArrayAttrib(uint32_t index);
    void disableArrayAttrib(uint32_t index);
    void setArrayAttrib(uint32_t index, const ArrayAttrib &, uint32_t bindingindex);
    void bind() const;
    void release();
private:
    GLuint m_vao;
};

#endif