#include "GameObject.h"



GameObject::GameObject()
{
}

GameObject::~GameObject()
{
}

void GameObject::addComponent(Component* component){
	GObjComponents.push_back(component);
}


void GameObject::Update() {

	for (int i = 0; i < GObjComponents.size(); i++) {
		GObjComponents[i]->Update();
	}

}