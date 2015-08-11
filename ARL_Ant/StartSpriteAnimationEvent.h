#pragma once

#include "IEvent.h"

class StartSpriteAnimationEvent : public IEvent
{
public:
	StartSpriteAnimationEvent(unsigned int entityId)
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