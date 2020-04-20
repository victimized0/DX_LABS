#ifndef _PONG_H_
#define _PONG_H_

#include "Engine.h"
#include "GameLogic.h"
#include "Input/Keyboard.h"

#pragma comment(lib, "DX_Engine.lib")

using Microsoft::WRL::ComPtr;
using DirectX::SimpleMath::Vector2;
using DirectX::SimpleMath::Vector3;
using DirectX::SimpleMath::Vector4;
using DirectX::SimpleMath::Matrix;
using DirectX::SimpleMath::Quaternion;

class Game final : public Engine {
public:
						Game(HINSTANCE hInstance);
	virtual				~Game();

						Game(const Game&)				= delete;
						Game(Game&&)					= delete;
						Game& operator=(const Game&)	= delete;
						Game& operator=(Game&&)			= delete;

	bool				Initialize(int iconId, int width, int height)final;

private:
	void				CreateScene();
	void				Update(float dt)final;

private:
	std::unique_ptr<DirectX::Keyboard>	m_keyboard;
	GameLogic							m_gameLogic;
	POINT								m_lastMousePos;
	float								m_paddleSpeed;
};

#endif //_PONG_H_
