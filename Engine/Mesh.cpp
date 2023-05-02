#include "Mesh.h"






Mesh::Mesh(const void* _VertexData, unsigned int _VertexDataSize, const VertexBufferLayout& _layout, const unsigned int* _IndexData, unsigned int _IndexCount)
{
	layout = _layout;
	vertexData = _VertexData;
	vertexDataSize = _VertexDataSize;
	indexData = _IndexData;
	indexCount = _IndexCount;
	SetupBuffers();
}

Mesh::~Mesh()
{
}


void Mesh::SetupBuffers() {
	// create buffers/arrays
	glGenVertexArrays(1, &VAO);
	glCheckError();
	glGenBuffers(1, &VBO);
	glCheckError();
	glGenBuffers(1, &IBO);
	glCheckError();

	glBindVertexArray(VAO);
	glCheckError();
	// load data into vertex buffers
	glBindBuffer(GL_ARRAY_BUFFER, VBO);
	glCheckError();
	glBufferData(GL_ARRAY_BUFFER, vertexDataSize, vertexData, GL_STATIC_DRAW);
	glCheckError();

	// load data into indices buffers
	glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, IBO);
	glCheckError();
	glBufferData(GL_ELEMENT_ARRAY_BUFFER, indexCount * sizeof(unsigned int), indexData, GL_STATIC_DRAW);
	glCheckError();



	const auto& elements = layout.getElements();
	unsigned int offset = 0;

	for (unsigned int i = 0; i < elements.size(); i++) {

		const auto& element = elements[i];
		glVertexAttribPointer(i, element.count, element.type, element.normalized, layout.getStride(), (const void*)offset);//this bounds to the currently binded buffer: app->vertexBufferObj
		glCheckError();
		glEnableVertexAttribArray(i);
		glCheckError();

		offset += element.count * VertexBufferElement::getSizeOfType(element.type);
	}

	glBindVertexArray(0);
	glCheckError();


}