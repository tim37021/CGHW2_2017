template <class T>
void Program::bindUniformBuffer(const std::string &name, GLuint bind_point_index, UniformBuffer<T> &ubo)
{
    GLuint block_index = glGetProgramResourceIndex(m_program, GL_UNIFORM_BLOCK, name.c_str());
    if(block_index==GL_INVALID_INDEX)
        return;
    glBindBufferBase(GL_UNIFORM_BUFFER, bind_point_index, ubo.id());
    GLenum err = glGetError();
    glUniformBlockBinding(m_program, block_index, bind_point_index);
}

template <class T>
void Program::bindShaderStorage(const std::string &name, GLuint bind_point_index, ShaderStorage<T> &ssbo)
{
    GLuint block_index = glGetProgramResourceIndex(m_program, GL_SHADER_STORAGE_BLOCK, name.c_str());
    GLenum err = glGetError();
    if(block_index==GL_INVALID_INDEX)
        return;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind_point_index, ssbo.id());
    err = glGetError();
    glShaderStorageBlockBinding(m_program, block_index, bind_point_index);
    err = glGetError();
    err = 0;
}
