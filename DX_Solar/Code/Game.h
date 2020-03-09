#ifndef _GAME_H_
#define _GAME_H_

#include "Engine.h"

#pragma comment(lib, "DX_Engine.lib")

using Microsoft::WRL::ComPtr;

class Game final : public Engine {
public:
	Game(HINSTANCE hInstance);
	virtual ~Game();

	Game(const Game&)					= delete;
	Game(Game&&)						= delete;
	Game& operator=(const Game&)		= delete;
	Game& operator=(Game&&)				= delete;

	virtual bool						Initialize()final;

private:
	void								OnEvent(const Event& event)final;
	void								Update(float dt)final;

	void								InitializeBuffers();

private:	
	POINT								m_lastMousePos;

};

#endif //_GAME_H_
