#ifndef _GAME_H_
#define _GAME_H_

#include "Engine.h"
#include "Events/Event.h"
#include "Events/MouseEvent.h"

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

	bool			Initialize(int iconId)final;

private:
	void			CreateScene();

	void			OnEvent(const Event& event)final;
	void			Update(float dt)final;

	void			OnMouseDown(const MouseEvent& event);
	void			OnMouseUp(const MouseEvent& event);
	void			OnMouseMove(const MouseEvent& event);
	void			OnMouseScroll(const MouseEvent& event);

private:	
	POINT			m_lastMousePos;

};

#endif //_GAME_H_
