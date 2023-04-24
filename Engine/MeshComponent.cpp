#include "MeshComponent.h"



MeshComponent::MeshComponent(Mesh* _mesh, Material* _material,GameObject* parentGameObject, const std::string componentName) : Component(parentGameObject,componentName) {
	mesh = _mesh;
	material = _material;

}