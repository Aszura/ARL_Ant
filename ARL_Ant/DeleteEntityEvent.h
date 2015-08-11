#pragma once

#include "IEvent.h"

class DeleteEntityEvent : public IEvent
{
public:
	DeleteEntityEvent(unsigned int entityId)
		: m_entityId(entityId)
	{
	}

	inline unsigned int getEntityId() const
	{
		return m_entityId;
	}
private:
	unsigned int m_entityId;
};