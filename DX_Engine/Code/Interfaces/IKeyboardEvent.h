#ifndef _INTERFACE_KEYBOARD_EVENT_H_
#define _INTERFACE_KEYBOARD_EVENT_H_

#include "../Events/KeyboardEvent.h"

class IKeyboardEvent {
public:
	virtual void OnKeyDown() = 0;
	virtual void OnKeyUp()   = 0;
};

#endif //_INTERFACE_KEYBOARD_EVENT_H_
