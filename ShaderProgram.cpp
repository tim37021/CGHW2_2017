#include "ShaderProgram.h"
#include <algorithm>
#include <glm/gtc/type_ptr.hpp>
#include <fstream>

UniformVariable::UniformVariable()
    : m_id(0)
{

}
UniformVariable::UniformVariable(GLint id)
    : m_id(id)
{

}

UniformVariable::UniformVariable(const UniformVariable &rhs)
    : m_id(rhs.m_id)
{
    
}

float UniformVariable::operator=(float val)
{
    glUniform1f(m_id, val);
    return val;
}
uint32_t UniformVariable::operator=(uint32_t val)
{
    glUniform1ui(m_id, val);
    return val;
}
int32_t UniformVariable::operator=(int32_t val)
{
    glUniform1i(m_id, val);
    return val;
}
glm::uvec2 UniformVariable::operator=(const glm::uvec2 &v)
{
    glUniform2uiv(m_id, 1, glm::value_ptr(v));
    return v;
}
glm::vec2 UniformVariable::operator=(const glm::vec2 &v)
{
    glUniform2fv(m_id, 1, glm::value_ptr(v));
    return v;
}
glm::vec3 UniformVariable::operator=(const glm::vec3 &v)
{
    glUniform3fv(m_id, 1, glm::value_ptr(v));
    return v;
}

glm::mat4 UniformVariable::operator=(const glm::mat4 &v)
{
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
    : m_program(prog, [](GLuint id){glDeleteProgram(id);})
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
    return m_program.get()!=0;
}

void Program::use() const
{
    glUseProgram(m_program.get());
}

void Program::dispatchCompute(uint32_t x, uint32_t y, uint32_t z) const
{
    glUseProgram(m_program.get());
    glDispatchCompute(x, y, z);
}

UniformVariable &Program::operator[](const std::string &name)
{
    // TODO
    use();
    auto it = m_uniformVariables.find(name);
    if(it==m_uniformVariables.cend()) {
        auto &obj = m_uniformVariables[name];
        obj = UniformVariable(glGetUniformLocation(m_program.get(), name.c_str()));
        return obj;
    } else
        return it->second;
}