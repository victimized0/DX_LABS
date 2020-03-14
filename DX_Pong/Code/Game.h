#ifndef _PONG_H_
#define _PONG_H_

#include "Engine.h"
#include "GameLogic.h"

#pragma comment(lib, "DX_Engine.lib")

static constexpr float GRID_TOP_BORDER		= 22.0f;
static constexpr float GRID_BOTTOM_BORDER	= -20.0f;
static constexpr float GRID_RIGHT_BORDER	= -30.0f;
static constexpr float GRID_LEFT_BORDER		= 30.0f;

using Microsoft::WRL::ComPtr;

class Game final : public Engine {
public:
						Game(HINSTANCE hInstance);
	virtual				~Game();

						Game(const Game&)				= delete;
						Game(Game&&)					= delete;
						Game& operator=(const Game&)	= delete;
						Game& operator=(Game&&)			= delete;

	bool				Initialize(int iconId)final;

private:
	void				CreateScene();
	void				CreateShaders();

	void				OnEvent(const Event& event)final;
	void				Update(float dt)final;

private:
	GameLogic			m_gameLogic;
	POINT				m_lastMousePos;
};

#endif //_PONG_H_
