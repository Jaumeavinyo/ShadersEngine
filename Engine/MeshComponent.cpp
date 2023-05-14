#include "MeshComponent.h"
#include "GeometryLoader.h"


void MeshComponent::Init() {

	printf("\n MeshComponent::Init()");


	mesh = new Mesh();
	LoadMesh(resourcePath);
	mesh->SetupSubmeshVAOs();
    




}
void MeshComponent::Update() {
	
	printf("\n MeshComponent::Update()");

	if (renderRequested()) {
		Render();
	}

}

void MeshComponent::Render() {

}

void MeshComponent::LoadMesh(const char* resourcePath) {//ASSIMP HERE

	LoadModel(app, this, resourcePath);
	
	
}