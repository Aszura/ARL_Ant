#pragma once

#include "IEvent.h"
#include "FieldComponent.h"
#include <vector>

class PathFinishedEvent : public IEvent
{
public:
	PathFinishedEvent(const std::vector<FieldComponent*>& path )
		: m_path(path)
	{
	}

	inline const std::vector<FieldComponent*>& getPath() const
	{
		return m_path;
	}
private:
	std::vector<FieldComponent*> m_path;
};