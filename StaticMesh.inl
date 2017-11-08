
template <class T>
void StaticMesh::setInstanceArray(ArrayBuffer<T> inst_arr)
{
    glBindVertexArray(vao);
    
    if(inst_arr.valid()) {
        inst_arr.bind();
        glEnableVertexAttribArray(3);
        glVertexAttribFormat(3, 3, GL_FLOAT, GL_FALSE, 0);
        glVertexAttribBinding(3, 3);
        glBindVertexBuffer(3, inst_arr.id(), 0, sizeof(float)*3);
        glVertexAttribDivisor(3, 1);
    } else {
        glDisableVertexAttribArray(3);
    }
    glBindVertexArray(0);
}