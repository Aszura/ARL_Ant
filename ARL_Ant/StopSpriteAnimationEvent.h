#pragma once

#include "IEvent.h"

class StopSpriteAnimationEvent : public IEvent
{
public:
	StopSpriteAnimationEvent(unsigned int entityId)
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