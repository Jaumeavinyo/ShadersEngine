#ifndef MESH_COMPONENT_H_
#define MESH_COMPONENT_H_


#include "GameObject.h"
#include "Component.h"
#include "Mesh.h"
#include "Material.h"

class MeshComponent : public Component
{
public:
	MeshComponent(App*_app,GameObject* parentGameObject, const std::string& componentName, const char* _resourcePath,bool _render = true)
		: Component(parentGameObject, componentName),app(_app),  resourcePath(_resourcePath), render(_render)
	{
		Init();
	}
	~MeshComponent() {};

	bool renderRequested()const { return render; }
	Mesh* getMesh()const { return mesh; }
	
	
	void Init();
	void Update();

	void Render();

	void LoadMesh(const char* resourcePath);


	App* app;
	
private:
	bool render;
	
	Mesh* mesh;

	std::vector<Vertex> vertices;
	

	unsigned int indices[6];
	VertexBufferLayout meshLayout;


private:
	const char* resourcePath;
};
#endif // !MESH_COMPONENT_H_
