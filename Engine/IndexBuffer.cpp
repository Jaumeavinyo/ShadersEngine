#include "IndexBuffer.h"
#include "errorHandler.h"



IndexBuffer::IndexBuffer(const unsigned int* data, unsigned int count) : m_Count(count){

    _ASSERT(sizeof(GLuint) == sizeof(unsigned int));

    glGenBuffers(1, &m_RendererID);
    glCheckError();
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glCheckError();
    glBufferData(GL_ELEMENT_ARRAY_BUFFER, count*sizeof(unsigned int), data, GL_STATIC_DRAW);
    glCheckError();
}


IndexBuffer::~IndexBuffer()
{
}



void IndexBuffer::bind()const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, m_RendererID);
    glCheckError();
}

void IndexBuffer::unbind()const {
    glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    glCheckError();
}

