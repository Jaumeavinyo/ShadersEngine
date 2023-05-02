#include "VertexArray.h"
#include "VertexBufferLayout.h"
#include "errorHandler.h"



VertexArray::VertexArray()
{
    glGenVertexArrays(1, &m_RendererID);
    glCheckError();
    if (m_RendererID == 0) {
        std::cerr << "Error: glGenVertexArrays() failed to generate a valid ID" << std::endl;
    }
}



VertexArray::~VertexArray()
{
    glDeleteVertexArrays(1, &m_RendererID);
    glCheckError();
}

void VertexArray::addBuffer(const VertexBuffer &vb, const VertexBufferLayout& layout)
{
    Bind();
	vb.bind();
    //const std::vector<VertexBufferElement>& elements = layout.getElements();
    const auto& elements = layout.getElements();
    unsigned int offset = 0;

    for (unsigned int i = 0; i < elements.size(); i++) {
       
        //const VertexBufferElement& element = elements[i];
        const auto& element = elements[i];
        glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);//this bounds to the currently binded buffer: app->vertexBufferObj
        glCheckError();
        glEnableVertexAttribArray(i);
        glCheckError();

        offset += element.count * VertexBufferElement::getSizeOfType(element.type);
    }
}

void VertexArray::Bind()const{
    glCheckError();
    if (m_RendererID != 0) {
        glBindVertexArray(m_RendererID);
        glCheckError();
    }
}


void VertexArray::unBind()const{
    glBindVertexArray(0);
    glCheckError();

}