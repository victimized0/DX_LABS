#ifndef _GAME_H_
#define _GAME_H_

#include "Engine.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"
#include "GameLogic.h"

#pragma comment(lib, "DX_Engine.lib")

using Microsoft::WRL::ComPtr;

static const char* NAME_GROUND = "Ground";
static const char* NAME_PLAYER = "Player";

static constexpr float GROUND_WIDTH		= 2000.0f;
static constexpr float GROUND_HEIGHT	= 2000.0f;

class Game final : public Engine {
public:
					Game(HINSTANCE hInstance);
	virtual			~Game();

					Game(const Game&)					= delete;
					Game(Game&&)						= delete;
					Game& operator=(const Game&)		= delete;
					Game& operator=(Game&&)				= delete;

	bool			Initialize(int iconId, int width, int height)final;

private:
	void			CreateScene();

	void			OnEvent(const Event& event)final;
	void			Update(float dt)final;

private:	
	GameLogic		m_gameLogic;

};

#endif //_GAME_H_
