#pragma once

#include "IEvent.h"
#include "GameStatusEnum.h"

class ChangeGameStatusEvent : public IEvent
{
public:
	ChangeGameStatusEvent(GameStatusEnum newStatus)
		: m_newStatus(newStatus)
	{
	}

	inline GameStatusEnum getNewStatus() const
	{
		return m_newStatus;
	}
private:
	GameStatusEnum m_newStatus;
};