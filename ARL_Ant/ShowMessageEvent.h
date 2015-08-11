#pragma once

#include "IEvent.h"
#include <string>

class ShowMessageEvent : public IEvent
{
public:
	ShowMessageEvent(std::string message)
		: m_message(message)
	{
	}

	inline std::string getMessage() const
	{
		return m_message;
	}
private:
	std::string m_message;
};