#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_

#include "Component.h"
#include "Mesh.h"
#include "Material.h"
class MeshComponent : public Component
{
public:
	MeshComponent(Mesh* mesh, Material* material, GameObject* parentGameObject, const std::string componentName);
	~MeshComponent();

	void Update();
private:
	Mesh* mesh;
	Material* material;

};
#endif // !MUSH_COMPONENT_H_
