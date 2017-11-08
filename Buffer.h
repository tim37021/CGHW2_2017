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

static inline AccessLevel operator |(const AccessLevel &lhs, const AccessLevel &rhs) {
    return static_cast<AccessLevel>(static_cast<int>(lhs) | static_cast<int>(rhs)); 
}

class Buffer {
public:
    Buffer();
    virtual void *map(AccessLevel level, uint32_t offset=0, uint32_t size=0);
    virtual void unmap();

    virtual void release();
    virtual void bind() const =0;
    virtual uint32_t size() const
    { return m_size; }

    bool valid() const;

    void allocate(AccessLevel level, uint32_t sizebyte, const void *data=nullptr);
    GLuint id() const;
protected:
    void bind(GLenum target) const;
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

    void allocateElements(AccessLevel level, uint32_t numElement, const T *data=nullptr);
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
    virtual void bind() const override;

    virtual uint32_t size() const override;
private:
    GLenum m_type;
};

// TODO inherit from TypedBuffer
template <class T>
class UniformBuffer: public Buffer {
public:
    UniformBuffer()=default;
    // shared buffer
    UniformBuffer(const Buffer &);
    // this can only map one structure
    T *mapStructure(AccessLevel level);
    void allocate(AccessLevel level=AccessLevel::eWrite, const T *data=nullptr);
    virtual void bind() const override;

    // this might not be 1 if shared
    virtual uint32_t size() const override;
    
    UniformBuffer &operator=(const UniformBuffer &rhs)=delete;
};

template <class T>
class ShaderStorage: public Buffer {
public:
    ShaderStorage()=default;
    ShaderStorage(const Buffer &);
    T *mapStructure(AccessLevel level);
    void allocate(AccessLevel level, const T *data=nullptr);
    virtual void bind() const override;

    // this might not be 1 if shared
    virtual uint32_t size() const override;
    
    ShaderStorage &operator=(const ShaderStorage &rhs)=delete;
};

#include "Buffer.inl"

#endif