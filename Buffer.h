#ifndef _BUFFER_H_
#define _BUFFER_H_

#include <GL/glew.h>
#include <cstdint>
#include <vector>

enum class AccessLevel {
    eDeviceLocal=0, eRead=1, eWrite=2, ePersistent=4, eCoherent=8
};

static inline AccessLevel operator &(const AccessLevel &lhs, const AccessLevel &rhs) {
    return static_cast<AccessLevel>(static_cast<int>(lhs) & static_cast<int>(rhs)); 
}

class Buffer {
public:
    Buffer();
    virtual void *map(AccessLevel level, uint32_t offset=0, uint32_t size=0);
    virtual void unmap();

    virtual void release();
    virtual void bind()=0;
    virtual uint32_t size() const
    { return m_size; }

    bool valid() const;

    void allocate(AccessLevel level, uint32_t sizebyte, const void *data=nullptr);
protected:
    void bind(GLenum target);
    Buffer(GLuint id);
private:
    GLuint m_id;
    GLuint m_size;
    void *m_ptr;
    bool m_immutable;
};

template <class T>
class TypedBuffer: public Buffer {
public:
    template<class... Args>
    TypedBuffer(Args&&... args);
    // note this is element based
    T *mapElements(AccessLevel level, uint32_t offset=0, uint32_t size=0);

    void allocateElements(AccessLevel level, uint32_t numElement, const T *data);
};

enum class ArrayBufferType {
    eVertex, eIndex
};

template <class T>
class ArrayBuffer: public TypedBuffer<T> {
public:
    ArrayBuffer();
    ArrayBuffer(ArrayBufferType type);
    template <class S>
    static ArrayBuffer<S> CreateFromSTDVector(ArrayBufferType type, const std::vector<S> &, AccessLevel level=AccessLevel::eDeviceLocal);
    virtual void bind() override;

    virtual uint32_t size() const override;
private:
    GLenum m_type;
};

#include "Buffer.inl"

#endif