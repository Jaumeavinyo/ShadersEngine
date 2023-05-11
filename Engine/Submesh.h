#pragma once

#include <stdio.h>
#include "VertexBufferLayout.h"

class Submesh
{
public:
	VertexBufferLayout Layout;

	std::vector<float> vertices;
	std::vector<unsigned int> indices;

	unsigned int vertexOffset;
	unsigned int indexOffset;
	
	std::vector<VAO> vaos;
};

