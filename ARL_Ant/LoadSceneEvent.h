#pragma once

#include "IEvent.h"

class LoadSceneEvent : public IEvent
{
public:
	LoadSceneEvent(unsigned int entityId, const char* scenePath)
		: m_entityId(entityId)
		, m_scenePath(scenePath)
	{
	}

	inline unsigned int getEntityId() const
	{
		return m_entityId;
	}
	inline const char* getScenePath() const
	{
		return m_scenePath;
	}
private:
	unsigned int m_entityId;
	const char* m_scenePath;
};