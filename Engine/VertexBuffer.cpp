#include "VertexBuffer.h"

#include "errorHandler.h"



VertexBuffer::VertexBuffer(const void* data, unsigned int size){
    glGenBuffers(1, &m_RendererID);
    glCheckError();
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glCheckError();
    glBufferData(GL_ARRAY_BUFFER, size, data, GL_STATIC_DRAW);
    glCheckError();
}


VertexBuffer::~VertexBuffer(){

    glDeleteBuffers(1, &m_RendererID);
    glCheckError();
}





void VertexBuffer::bind()const {
    glBindBuffer(GL_ARRAY_BUFFER, m_RendererID);
    glCheckError();
}

void VertexBuffer::unbind()const {
    glBindBuffer(GL_ARRAY_BUFFER, 0);
    glCheckError();
}
