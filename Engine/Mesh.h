#include "VertexBuffer.h"
#include "IndexBuffer.h"
#include "VertexArray.h"

struct Mesh
{
	VertexBufferLayout attrLayout;

	VertexArray va;
	VertexBuffer vb;
	IndexBuffer ib;
};