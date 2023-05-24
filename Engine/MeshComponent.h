#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_


#include "GameObject.h"
#include "Component.h"
#include "GLclassesAndStructs.h"


class MeshComponent : public Component
{
public:
	MeshComponent(GameObject* parentGameObject, const std::string& componentName, Mesh* meshPtr, bool _render = true)
		: Component(parentGameObject, componentName), mesh(meshPtr), render(_render)
	{}
	~MeshComponent() {};

	bool renderRequested()const { return render; }
	
	void Update();

	

private:
	bool render;
	Mesh* mesh;


};
#endif // !MESH_COMPONENT_H_
