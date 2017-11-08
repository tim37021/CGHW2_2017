template <class T>
ArrayBuffer<T>::ArrayBuffer()
    : TypedBuffer(0)
{

}

template <class T>
template<class... Args>
TypedBuffer<T>::TypedBuffer(Args&&... args)
    : Buffer(args...)
{

}

template <class T>
T *TypedBuffer<T>::mapElements(AccessLevel level, uint32_t offset, uint32_t size)
{
    return reinterpret_cast<T *>(Buffer::map(level, offset*sizeof(T), size*sizeof(T)));
}

template <class T>
void TypedBuffer<T>::allocateElements(AccessLevel level, uint32_t numElement, const T *data)
{
    Buffer::allocate(level, numElement*sizeof(T), data);
}

template <class T>
ArrayBuffer<T>::ArrayBuffer(ArrayBufferType type)
{
    switch(type) {
        case ArrayBufferType::eVertex:
            m_type = GL_ARRAY_BUFFER; break;
        case ArrayBufferType::eIndex:
            m_type = GL_ELEMENT_ARRAY_BUFFER; break;
    }
}

template <class T>
template <class S>
ArrayBuffer<S> ArrayBuffer<T>::CreateFromSTDVector(ArrayBufferType type, const std::vector<S> &src, AccessLevel level)
{
    ArrayBuffer<S> buffer(type);

    buffer.allocateElements(level, src.size(), src.data());

    return buffer;
}

template <class T>
void ArrayBuffer<T>::bind() const
{
    Buffer::bind(m_type);
}

template <class T>
uint32_t ArrayBuffer<T>::size() const
{
    // this will not work when user allocate on thier own
    return Buffer::size()/sizeof(T);
}

template <class T>
UniformBuffer<T>::UniformBuffer(const Buffer &b2shared)
    : Buffer(b2shared.id())
{

}

template <class T>
T *UniformBuffer<T>::mapStructure(AccessLevel level)
{
    return reinterpret_cast<T *>(Buffer::map(level, 0, sizeof(T)));
}

template <class T>
void UniformBuffer<T>::allocate(AccessLevel level, const T *data)
{
    Buffer::allocate(level, sizeof(T), data);
}

template <class T>
uint32_t UniformBuffer<T>::size() const
{
    return Buffer::size()/sizeof(T);
}

template <class T>
void UniformBuffer<T>::bind() const
{
    Buffer::bind(GL_UNIFORM_BUFFER);
}

template <class T>
ShaderStorage<T>::ShaderStorage(const Buffer &b2shared)
    : Buffer(b2shared.id())
{

}


template <class T>
T *ShaderStorage<T>::mapStructure(AccessLevel level)
{
    return reinterpret_cast<T *>(Buffer::map(level, 0, sizeof(T)));
}

template <class T>
void ShaderStorage<T>::allocate(AccessLevel level, const T *data)
{
    Buffer::allocate(level, sizeof(T), data);
}

template <class T>
void ShaderStorage<T>::bind() const
{
    Buffer::bind(GL_SHADER_STORAGE_BUFFER);
}

template <class T>
uint32_t ShaderStorage<T>::size() const
{
    return Buffer::size()/sizeof(T);
}
