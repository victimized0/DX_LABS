#ifndef _GAME_LOGIC_H_
#define _GAME_LOGIC_H_

#include "StepTimer.h"
#include "Scene.h"
#include "Events/Event.h"
#include "SceneObjects/Camera.h"

static const char* NAME_LPADDLE				= "lPaddle";
static const char* NAME_RPADDLE				= "rPaddle";
static const char* NAME_BALL				= "ball";

static constexpr float GRID_TOP_BORDER		= 22.0f;
static constexpr float GRID_BOTTOM_BORDER	= -20.0f;
static constexpr float GRID_LEFT_BORDER		= 30.0f;
static constexpr float GRID_RIGHT_BORDER	= -30.0f;

class GameLogic {
public:

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
