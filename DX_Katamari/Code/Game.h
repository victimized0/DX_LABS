#ifndef _GAME_H_
#define _GAME_H_

#include "Engine.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

#pragma comment(lib, "DX_Engine.lib")

using Microsoft::WRL::ComPtr;

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

};

#endif //_GAME_H_
