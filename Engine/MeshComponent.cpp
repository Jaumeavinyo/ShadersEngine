#include "MeshComponent.h"



void MeshComponent::Init() {

	printf("\n MeshComponent::Init()");
	LoadvertexData(resourcePath);
	
    mesh = new Mesh(vertices, meshLayout, indices, 6);




}
void MeshComponent::Update() {
	
	printf("\n MeshComponent::Update()");

	if (renderRequested()) {
		Render();
	}

}

void MeshComponent::Render() {

}

void MeshComponent::LoadvertexData(const char* resourcePath) {//ASSIMP HERE

	printf("\n MeshComponent::LoadvertexData()");

	std::vector<Vertex> _vertices = {
	   {glm::vec3(-0.5,-0.5,0.0),glm::vec2(0.0,0.0)},
	   {glm::vec3(0.5,-0.5,0.0),glm::vec2(1.0,0.0)},
	   {glm::vec3(0.5,0.5,0.0),glm::vec2(1.0,1.0)},
	   {glm::vec3(-0.5,0.5,0.0),glm::vec2(0.0,1.0)}
	};

	unsigned int _indices[6] = {
		0,1,2,
		2,3,0
	};
	unsigned int numIndices = 6;

	vertices = _vertices;
	std::copy(_indices, _indices + numIndices, indices);


	VertexBufferLayout _layout;
	_layout.Push<float>(3);//first element of the stride: 2 floats for position
	_layout.Push<float>(2);//second element 2 floats for uvg texcorrds

	meshLayout = _layout;
}