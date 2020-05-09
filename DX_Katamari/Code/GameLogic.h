#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "StepTimer.h"
#include "Scene.h"
#include "SceneObjects/Camera.h"

class GameLogic {
public:

public:
				GameLogic()								= default;
				~GameLogic()							= default;

				GameLogic(GameLogic const&)				= delete;
				GameLogic& operator=(GameLogic const&)	= delete;
				GameLogic(GameLogic&&)					= delete;
				GameLogic& operator=(GameLogic&&)		= delete;

	bool		Initialise(Scene* pScene);
	void		Update(float dt);

private:
	Scene*		m_pScene;

};

#endif //_GAME_LOGIC_H_
