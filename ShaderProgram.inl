template <class T>
void Program::bindUniformBuffer(const std::string &name, GLuint bind_point_index, UniformBuffer<T> &ubo)
{
    GLuint block_index = glGetProgramResourceIndex(m_program, GL_UNIFORM_BLOCK, name.c_str());
    if(block_index==GL_INVALID_INDEX)
        return;
    glBindBufferBase(GL_UNIFORM_BUFFER, bind_point_index, ubo.id());
    glUniformBlockBinding(m_program, block_index, bind_point_index);
}
/*
template <class T>
void Program::bindShaderStorage(const std::string &name, GLuint bind_point_index, ShaderStorage<T> &ubo)
{
    GLuint block_index = glGetProgramResourceIndex(m_program, GL_SHADER_STORAGE_BLOCK, name.c_str());
    if(block_index==GL_INVALID_INDEX)
        return;
    glBindBufferBase(GL_SHADER_STORAGE_BUFFER, bind_point_index, ubo.id());
    glShaderStorageBlockBinding(m_program, block_index, bind_point_index);
}
*/