
template <class T>
void StaticMesh::setInstanceArray(ArrayBuffer<T> inst_arr)
{
    glBindVertexArray(vao);
    
    if(inst_arr.valid()) {
        inst_arr.bind();
        glEnableVertexAttribArray(3);
        glVertexAttribPointer(3, sizeof(T)/sizeof(GLfloat), GL_FLOAT, GL_FALSE, 0, 0);
        glVertexAttribDivisor(3, 1);
    } else {
        glDisableVertexAttribArray(3);
    }
    glBindVertexArray(0);
}