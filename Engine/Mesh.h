
#include "VertexBufferLayout.h"
class Mesh
{
public:
	Mesh(const void* VertexData, unsigned int VertexDataSize, const VertexBufferLayout& _layout, const unsigned int* IndexData, unsigned int IndexCount);
	~Mesh();

	void SetupBuffers();

	unsigned int getVAO()const { return VAO; }
	unsigned int getVBO()const { return VBO; }
	unsigned int getIBO()const { return IBO; }

	VertexBufferLayout layout;

	
	const void* vertexData;
	unsigned int vertexDataSize;

	const unsigned int* indexData;
	unsigned int indexCount;


private:

	unsigned int VAO;
	unsigned int VBO;
	unsigned int IBO;
};
