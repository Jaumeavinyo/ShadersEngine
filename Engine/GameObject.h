#ifndef GAME_OBJECT_H_
#define GAME_OBJECT_H_
#include "Component.h"
#include <vector>



class GameObject
{
public:
	GameObject();
	~GameObject();

	Component* getComponent(unsigned int i) { return GObjComponents[i]; }
	
private:

	std::vector<Component*> GObjComponents;

};



#endif // !GAME_OBJECT_H_
