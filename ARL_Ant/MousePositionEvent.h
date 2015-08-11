#pragma once

#include "IEvent.h"
#include <glm\glm.hpp>

class MousePositionEvent : public IEvent
{
public:
	MousePositionEvent(glm::ivec2 mousePosition)
		: m_mousePosition(mousePosition)
	{
	}

	inline glm::ivec2 getMousePosition() const
	{
		return m_mousePosition;
	}
private:
	glm::ivec2 m_mousePosition;
};