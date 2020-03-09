#ifndef _EVENT_H_
#define _EVENT_H_

enum class EventType : unsigned char {
	None,
	MouseUp,
	MouseDown,
	MouseMove,
	MouseScroll,
	KeyDown,
	Total
};

class Event {
public:
	Event(EventType type) : m_eventType(type) {}

	Event(Event const&)				= delete;
	Event operator=(Event const&)	= delete;
	Event(Event&&)					= delete;
	Event operator=(Event&&)		= delete;

	EventType GetType()const {
		return m_eventType;
	}

private:
	EventType m_eventType;
};

#endif //_EVENT_H_
