#pragma once

class IEvent;

class IEventListener
{
public:
	virtual ~IEventListener() {}
	virtual void EventListener(IEvent* eventData) = 0;
};