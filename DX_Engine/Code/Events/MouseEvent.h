#ifndef _MOUSE_EVENT_H_
#define _MOUSE_EVENT_H_

#include "Event.h"

enum class MouseKeyState : unsigned char {
	None,
	LButton,
	RButton
};

class MouseEvent : public Event {
public:
	MouseEvent(EventType event, WPARAM wParam, int x, int y)
		: Event(event)
		, m_keyState(MouseKeyState::None)
		, m_posX(x)
		, m_posY(y)
		, m_wParam(wParam)
	{
		if ((wParam & MK_LBUTTON) != 0) {
			m_keyState = MouseKeyState::LButton;
		}

		if ((wParam & MK_RBUTTON) != 0) {
			m_keyState = MouseKeyState::RButton;
		}
	}

	MouseEvent(WPARAM wParam, int x, int y)
		: MouseEvent(EventType::None, wParam, x, y)
	{}

	MouseEvent(EventType event, WPARAM wParam)
		: MouseEvent(event, wParam, 0, 0)
	{}

	MouseKeyState	GetKeyState()const	{ return m_keyState; }
	int				GetPosX()const		{ return m_posX; }
	int				GetPosY()const		{ return m_posY; }
	WPARAM			GetParam()const		{ return m_wParam; }

private:
	MouseKeyState	m_keyState;
	WPARAM			m_wParam;
	int				m_posX;
	int				m_posY;
};

#endif //_MOUSE_EVENT_H_
