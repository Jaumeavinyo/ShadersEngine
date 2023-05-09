
#include "VertexBufferLayout.h"
#include "Code/engine.h"
class Mesh
{
public:
	Mesh(std::vector<Vertex> _vertices, const VertexBufferLayout& _layout, const unsigned int* IndexData, unsigned int IndexCount,unsigned int drawMode = GL_STATIC_DRAW);
	~Mesh();

	void SetupBuffers();
	void SetupLayout();

	unsigned int getVAO()const { return VAO; }
	unsigned int getVBO()const { return VBO; }
	unsigned int getIBO()const { return IBO; }

	VertexBufferLayout layout;

	
	std::vector<Vertex> vertices;

	const unsigned int* indexData;
	unsigned int indexCount;


private:

	unsigned int VAO;
	unsigned int drawMode;

	unsigned int VBO;
	unsigned int IBO;
};
