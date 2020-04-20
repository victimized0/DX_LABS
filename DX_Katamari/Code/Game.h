#ifndef _GAME_H_
#define _GAME_H_

#include "Engine.h"
#include "GameLogic.h"

#include "Input/Mouse.h"
#include "Input/Keyboard.h"

#pragma comment(lib, "DX_Engine.lib")

using Microsoft::WRL::ComPtr;

static const char* NAME_TERRAIN = "Ground";
static const char* NAME_PLAYER  = "Katamari";

static std::string MeshesPath	= "Art/Meshes/";
static std::string TexturesPath	= "Art/Textures/";

static constexpr float TERRAIN_SIZE = 1000.0f;

class Game final : public Engine {
private:
	typedef std::string string;

public:
										Game(HINSTANCE hInstance);
	virtual								~Game();

										Game(const Game&)					= delete;
										Game(Game&&)						= delete;
										Game& operator=(const Game&)		= delete;
										Game& operator=(Game&&)				= delete;

	bool								Initialize(int iconId, int width, int height)final;

private:
	void								CreateScene();
	void								GeneratePickups();
	void								Update(float dt)final;

	void								OnMouseUpdate(float dt);
	void								OnKeyboardUpdate(float dt);

private:
	std::unique_ptr<DirectX::Mouse>		m_mouse;
	std::unique_ptr<DirectX::Keyboard>	m_keyboard;
	GameLogic							m_gameLogic;

};

#endif //_GAME_H_
