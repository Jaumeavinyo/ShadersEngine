#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_


#include "Component.h"

#include "Mesh.h"
#include "Material.h"

class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* parentGameObject, const std::string& componentName, Mesh* meshPtr, Material* materialPtr)
		: Component(parentGameObject, componentName), mesh(meshPtr), material(materialPtr)
	{}
	~MeshComponent() ;

	void Update();
private:
	Mesh* mesh;
	Material* material;

};
#endif // !MESH_COMPONENT_H_
