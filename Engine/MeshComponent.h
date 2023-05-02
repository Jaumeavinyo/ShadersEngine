#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_


#include "GameObject.h"
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* parentGameObject, const std::string& componentName, Mesh* meshPtr, Material* materialPtr, bool _render = true)
		: Component(parentGameObject, componentName), mesh(meshPtr), material(materialPtr), render(_render)
	{}
	~MeshComponent() {};

	bool renderRequested()const { return render; }
	Mesh* getMesh()const{return mesh;}
	void Update();

	VertexArray& getVA()const { return mesh->va; }
	VertexBuffer& getVB()const { return mesh->vb; }
	IndexBuffer& getIB()const { return mesh->ib; }

private:
	bool render;
	Mesh* mesh;
	Material* material;

};
#endif // !MESH_COMPONENT_H_
