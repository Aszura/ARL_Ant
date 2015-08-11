#pragma once

#include "IEvent.h"
#include <glm\glm.hpp>

class ResizeScreenEvent : public IEvent
{
public:
	ResizeScreenEvent(glm::ivec2 screenSize)
		: m_screenSize(screenSize)
	{
	}

	inline glm::ivec2 getScreenSize() const
	{
		return m_screenSize;
	}
private:
	glm::ivec2 m_screenSize;
};