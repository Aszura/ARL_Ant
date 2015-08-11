#pragma once

#include "IEvent.h"
//#include "EventType.h"
#include "BaseComponent.h"

class RegisterComponentEvent : public IEvent
{
public:
	RegisterComponentEvent(BaseComponent* component)
		: m_component(component)
	{
	}

	//inline EventType getEventType() const
	//{
	//	return REGISTER_COMPONENT;
	//}
	inline BaseComponent* getComponent() const
	{
		return m_component;
	}
private:
	BaseComponent* m_component;
};