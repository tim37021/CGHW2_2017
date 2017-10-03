#ifndef _SHADER_PROGRAM_H_
#define _SHADER_PROGRAM_H_

#include <cstdint>
#include <GL/glew.h>
#include <map>
#include <functional>
#include <glm/glm.hpp>

using Vector2u = glm::uvec2;
using Vector2f = glm::vec2;
using Vector3f = glm::vec3;
using Matrix4 = glm::mat4;

class UniformVariable
{
public:
    UniformVariable();
    UniformVariable(GLint id);
    UniformVariable(const UniformVariable &rhs);
    float operator=(float);
    uint32_t operator=(uint32_t);
    int32_t operator=(int32_t);
    Vector2u operator=(const Vector2u &);
    Vector2f operator=(const Vector2f &);
    Vector3f operator=(const Vector3f &);
    Matrix4 operator=(const Matrix4 &);
    UniformVariable &operator=(const UniformVariable &rhs)=default;
    bool valid() const;
private:
    GLint m_id;
};

template <class T>
class RefCounting {
public:
    RefCounting();
    RefCounting(T data, std::function<void (T)> deleter);
    RefCounting(const RefCounting<T> &ref);
    ~RefCounting();
    T get() const
    { return m_data; }

    RefCounting<T> &operator=(const RefCounting<T> &rhs);
private:
    T m_data;
    uint32_t *m_counter;
    std::function<void (T)> m_deleter;
};

template <class T>
RefCounting<T>::RefCounting()
    : m_counter(nullptr)
{

}

template <class T>
RefCounting<T>::RefCounting(T data, std::function<void (T)> deleter)
    : m_data(data), m_deleter(deleter), m_counter(new uint32_t(1))
{

}
template <class T>
RefCounting<T>::RefCounting(const RefCounting<T> &ref)
{
    m_data = ref.m_data;
    m_deleter = ref.m_deleter;
    m_counter = ref.m_counter;
    (*m_counter)++;
}

template <class T>
RefCounting<T>::~RefCounting()
{
    if(m_counter==nullptr)
        return;
    (*m_counter)--;
    if(!*m_counter) {
        m_deleter(m_data);
        delete m_counter;
    }
}

template <class T>
RefCounting<T> &RefCounting<T>::operator=(const RefCounting<T> &rhs)
{
    if(this!=&rhs) {
        this->~RefCounting();

        m_data = rhs.m_data;
        m_deleter = rhs.m_deleter;
        m_counter = rhs.m_counter;
        (*m_counter)++;
    }
    return *this;
}

class Program
{
public:
    Program()=default;
    Program(const Program &)=default;
    static Program LoadFromFile(const std::string &filename);
    static Program LoadFromFile(const std::string &vs, const std::string &fs);
    UniformVariable &operator[](const std::string &);

    bool valid() const;
    void use() const;

    void dispatchCompute(uint32_t x, uint32_t y, uint32_t z) const;
private:
    Program(GLuint prog);
    std::map<std::string, UniformVariable> m_uniformVariables;
    RefCounting<GLuint> m_program;
};

#endif