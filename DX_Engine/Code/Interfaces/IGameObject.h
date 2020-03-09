#ifndef _INTERFACE_GAME_OBJECT_H_
#define _INTERFACE_GAME_OBJECT_H_

class IGameObject {
	virtual void Initialise() = 0;
	virtual void Update(float dt) = 0;
};

#endif //_INTERFACE_GAME_OBJECT_H_
