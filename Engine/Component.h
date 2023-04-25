#ifndef COMPONENT_H_
#define COMPONENT_H_


#include <iostream>



class GameObject;

class Component
{
public:

	Component(GameObject* parentGameObject,const std::string componentName):parentGameObject(parentGameObject),name(componentName){}
	virtual ~Component() {};

	

	virtual void Update() {};

	GameObject& getParentGameObject()const { return *parentGameObject; }
	std::string getName()const { return name; }
private:

	std::string name;
	GameObject* parentGameObject;

};






#endif // !COMPONENT_H_



