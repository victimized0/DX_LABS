#ifndef _KEYBOARD_EVENT_H_
#define _KEYBOARD_EVENT_H_

#include "Event.h"

enum class KeyCode : unsigned char {
	None,
	W,
	A,
	S,
	D,
	Up,
	Down,
	Left,
	Right,
	Escape
};

class KeyboardEvent : public Event {
public:
	KeyboardEvent(EventType event, WPARAM wParam)
		: Event(event)
	{
		switch (wParam) {
			case VK_ESCAPE:		m_keyCode = KeyCode::Escape;	break;
			case VK_UP:			m_keyCode = KeyCode::Up;		break;
			case VK_DOWN:		m_keyCode = KeyCode::Down;		break;
			case VK_LEFT:		m_keyCode = KeyCode::Left;		break;
			case VK_RIGHT:		m_keyCode = KeyCode::Right;		break;
			case 0x57:			m_keyCode = KeyCode::W;			break;
			case 0x41:			m_keyCode = KeyCode::A;			break;
			case 0x53:			m_keyCode = KeyCode::S;			break;
			case 0x44:			m_keyCode = KeyCode::D;			break;
			default:			m_keyCode = KeyCode::None;		break;
		}
	}

	KeyboardEvent(WPARAM wParam)
		: KeyboardEvent(EventType::None, wParam)
	{}

	KeyCode GetKeyCode() {
		return m_keyCode;
	}

private:
	KeyCode m_keyCode;

};

#endif //_KEYBOARD_EVENT_H_
