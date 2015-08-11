#pragma once

#include "IEvent.h"
#include "FieldComponent.h"

class StartSetEvent : public IEvent
{
public:
	StartSetEvent(FieldComponent* start)
		: m_start(start)
	{
	}

	inline FieldComponent* getStartField() const
	{
		return m_start;
	}

private:
	FieldComponent* m_start;
};