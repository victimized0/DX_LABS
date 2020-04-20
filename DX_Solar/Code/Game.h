#ifndef _GAME_H_
#define _GAME_H_

#include "Engine.h"
#include "Input/Mouse.h"

#pragma comment(lib, "DX_Engine.lib")

using Microsoft::WRL::ComPtr;

static const char* NAME_SUN		= "The Sun";
static const char* NAME_EARTH	= "The Earth";
static const char* NAME_MOON	= "The Moon";

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
	void			Update(float dt)final;

private:
	std::unique_ptr<DirectX::Mouse>	m_mouse;

};

#endif //_GAME_H_
