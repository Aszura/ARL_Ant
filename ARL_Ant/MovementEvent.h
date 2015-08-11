#pragma once

#include "IEvent.h"
#include <glm\glm.hpp>

class MovementEvent : public IEvent
{
public:
	MovementEvent(unsigned int entityId, glm::vec2 translation, float rotation)
		: m_entityId(entityId)
		, m_translation(translation)
		, m_rotation(rotation)
	{
	}
	MovementEvent(unsigned int entityId, glm::vec2 axis, float distance, float rotation)
		: m_entityId(entityId)
		, m_translation(axis * distance)
		, m_rotation(rotation)
	{
	}

	inline unsigned int getEntityId() const
	{
		return m_entityId;
	}
	inline const glm::vec2& getTranslation() const
	{
		return m_translation;
	}
	inline float getRotation() const
	{
		return m_rotation;
	}
private:
	unsigned int m_entityId;
	glm::vec2 m_translation;
	float m_rotation;
};