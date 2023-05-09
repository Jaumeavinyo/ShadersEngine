#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_


#include "GameObject.h"
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* parentGameObject, const std::string& componentName,  Material* materialPtr, const char* _resourcePath,bool _render = true)
		: Component(parentGameObject, componentName),  material(materialPtr), resourcePath(_resourcePath), render(_render)
	{}
	~MeshComponent() {};

	bool renderRequested()const { return render; }
	Mesh* getMesh()const{return mesh;}
	Material* getMaterial()const { return material; }
	
	void Init();
	void Update();

	void Render();

	void LoadvertexData(const char* resourcePath);

private:
	bool render;
	Mesh* mesh;
	Material* material;

	std::vector<Vertex> vertices;
	   
	unsigned int indices[6];
	VertexBufferLayout meshLayout;


private:
	const char* resourcePath;
};
#endif // !MESH_COMPONENT_H_
