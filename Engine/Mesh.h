
#include "VertexBufferLayout.h"
#include "Buffers.h"
#include "Code/engine.h"
#include "Submesh.h"

class Mesh
{
public:
	Mesh( unsigned int drawMode = GL_STATIC_DRAW);
	~Mesh();

	void SetupSubmeshVAOs();
	void SetupLayout();
	void CreateVAOforSubmeshes();
	
	unsigned int getVBO()const { return VBOhandle; }
	unsigned int getIBO()const { return IBOhandle; }

	std::vector<unsigned int>* getSubMeshMaterialsIDx() { return &SubmeshMaterialsIDx; }


private:

	unsigned int drawMode;

	

public:
	
	unsigned int VBOhandle;
	unsigned int IBOhandle;

	std::vector<unsigned int> SubmeshMaterialsIDx;
	std::vector<Material> materials;
	std::vector<Submesh> submeshes;
};
