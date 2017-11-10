
template <class T>
void StaticMesh::setInstanceArray(ArrayBuffer<T> inst_arr, uint32_t size, uint32_t offset)
{
    
    if(inst_arr.valid()) {
        ArrayAttrib attrib_inst;
        attrib_inst.format(size, AttribArrayType::eFloat, 0);
        AttribBufferBinding inst_bb(3, inst_arr);
        inst_bb.divisor(1);
        attrib_inst.bind(inst_bb, sizeof(T), offset);
        m_drawState.enableArrayAttrib(3);
        m_drawState.setArrayAttrib(3, attrib_inst);

    } else {
        m_drawState.disableArrayAttrib(3);
    }
}