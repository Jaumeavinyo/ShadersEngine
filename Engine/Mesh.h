
#include "VertexBufferLayout.h"
#include "Buffers.h"
#include "Code/engine.h"
#include "Submesh.h"

class Mesh
{
public:
	Mesh(std::vector<Vertex> _vertices, const VertexBufferLayout& _layout, const unsigned int* IndexData, unsigned int IndexCount,unsigned int drawMode = GL_STATIC_DRAW);
	~Mesh();

	void SetupBuffers();
	void SetupLayout();
	void CreateVAOforSubmeshes();
	
	unsigned int getVBO()const { return VBOhandle; }
	unsigned int getIBO()const { return IBOhandle; }

	std::vector<unsigned int>* getSubMeshMaterialsIDx() { return &SubmeshMaterialsIDx; }

	
	std::vector<Vertex> vertices;
	
	const unsigned int* indexData;
	unsigned int indexCount;


private:

	unsigned int drawMode;

	

public:
	
	unsigned int VBOhandle;
	unsigned int IBOhandle;

	std::vector<unsigned int> SubmeshMaterialsIDx;
	std::vector<Material> materials;
	std::vector<Submesh> submeshes;
};
