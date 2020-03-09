#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "StepTimer.h"
#include "Scene.h"
#include "Events/Event.h"
#include "SceneObjects/Camera.h"

class GameLogic {
public:
				GameLogic();
				~GameLogic();

				GameLogic(GameLogic const&)				= delete;
				GameLogic& operator=(GameLogic const&)	= delete;
				GameLogic(GameLogic&&)					= delete;
				GameLogic& operator=(GameLogic&&)		= delete;

	bool		Initialise(Scene* pScene);
	void		OnEvent(const Event& event);
	void		Update(float dt);

private:
	Scene*		m_pScene;

};

#endif //_GAME_LOGIC_H_
