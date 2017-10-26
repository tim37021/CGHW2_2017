#include "ShaderProgram.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

UniformVariable::UniformVariable()
    : m_id(0)
{

}
UniformVariable::UniformVariable(const GLuint program, GLint id)
    : m_program(program), m_id(id)
{

}
float UniformVariable::operator=(float val)
{
    glUseProgram(m_program);
    glUniform1f(m_id, val);
    return val;
}
uint32_t UniformVariable::operator=(uint32_t val)
{
    glUseProgram(m_program);
    glUniform1ui(m_id, val);
    return val;
}
int32_t UniformVariable::operator=(int32_t val)
{
    glUseProgram(m_program);
    glUniform1i(m_id, val);
    return val;
}
glm::uvec2 UniformVariable::operator=(const glm::uvec2 &v)
{
    glUseProgram(m_program);
    glUniform2uiv(m_id, 1, glm::value_ptr(v));
    return v;
}
glm::vec2 UniformVariable::operator=(const glm::vec2 &v)
{
    glUseProgram(m_program);
    glUniform2fv(m_id, 1, glm::value_ptr(v));
    return v;
}
glm::vec3 UniformVariable::operator=(const glm::vec3 &v)
{
    glUseProgram(m_program);
    glUniform3fv(m_id, 1, glm::value_ptr(v));
    return v;
}

glm::mat4 UniformVariable::operator=(const glm::mat4 &v)
{
    glUseProgram(m_program);
    // mat4 of glm is column major, same as opengl
    // we don't need to transpose it. so..GL_FALSE
    glUniformMatrix4fv(m_id, 1, GL_FALSE, glm::value_ptr(v));
    return v;
}

bool UniformVariable::valid() const
{
    return m_id != -1;
}

static std::string read_file(const std::string &filename) 
{
	std::ifstream t(filename);
	return std::string((std::istreambuf_iterator<char>(t)),
		std::istreambuf_iterator<char>());
}

Program::Program(GLuint prog)
    : m_program(prog)
{

}

Program Program::LoadFromFile(const std::string &filename)
{

    GLuint shader=glCreateShader(GL_COMPUTE_SHADER);
	auto src = read_file(filename);
    int len = static_cast<int>(src.length());
    const char *c = src.c_str();
    glShaderSource(shader, 1, &c, &len);
    glCompileShader(shader);
    GLuint id = glCreateProgram();
    glAttachShader(id, shader);
    glLinkProgram(id);
    return Program(id);
}
#include <iostream>
Program Program::LoadFromFile(const std::string &vs, const std::string &fs)
{
	auto vs_src = ::read_file(vs);
	auto fs_src = ::read_file(fs);

    GLuint vshader=glCreateShader(GL_VERTEX_SHADER);
    GLuint fshader=glCreateShader(GL_FRAGMENT_SHADER);
    int len = static_cast<int>(vs_src.length());
    const char *c = vs_src.c_str();
    glShaderSource(vshader, 1, &c, &len);
    glCompileShader(vshader);
    char lg[1024];
    glGetShaderInfoLog(vshader, 1024, &len, lg);
    std::cerr<<lg;
    len = static_cast<int>(fs_src.length());
    c = fs_src.c_str();
    glShaderSource(fshader, 1, &c, &len);
    glCompileShader(fshader);
    GLuint id = glCreateProgram();
    glAttachShader(id, vshader);
    glAttachShader(id, fshader);
    glLinkProgram(id);

    glDeleteShader(vshader);
    glDeleteShader(fshader);
    return Program(id);
}


bool Program::valid() const
{
    return m_program!=0;
}

void Program::use() const
{
    glUseProgram(m_program);
}

void Program::dispatchCompute(uint32_t x, uint32_t y, uint32_t z) const
{
    glUseProgram(m_program);
    glDispatchCompute(x, y, z);
}

UniformVariable &Program::operator[](const std::string &name)
{
    // TODO
    use();
    auto it = m_uniformVariables.find(name);
    if(it==m_uniformVariables.cend()) {
        auto &obj = m_uniformVariables[name];
        obj = UniformVariable(m_program, glGetUniformLocation(m_program, name.c_str()));
        return obj;
    } else
        return it->second;
}

void Program::release()
{
    glDeleteProgram(m_program);
    m_program = 0;
}