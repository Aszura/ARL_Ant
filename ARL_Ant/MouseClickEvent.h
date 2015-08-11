#pragma once

#pragma once

#include "IEvent.h"
#include <SFML\Window.hpp>

class MouseClickEvent : public IEvent
{
public:
	MouseClickEvent(sf::Mouse::Button button)
		: m_button(button)
	{
	}

	inline sf::Mouse::Button getButton() const
	{
		return m_button;
	}

	inline bool getIsDown() const
	{
		return m_isDown;
	}
private:
	sf::Mouse::Button m_button;
	bool m_isDown;
};