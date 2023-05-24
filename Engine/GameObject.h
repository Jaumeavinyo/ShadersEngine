#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_

#include <vector>

#include "Component.h"

class GameObject
{
public:
	GameObject();
	~GameObject();

	Component* getComponent(unsigned int i)const { return GObjComponents[i]; }
	void addComponent(Component* component);


	void Update();
private:

	std::vector<Component*> GObjComponents;

};



#endif // !GAME_OBJECT_H_
