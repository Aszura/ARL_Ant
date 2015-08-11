#pragma once

#include "IEvent.h"
#include <SFML\Window.hpp>

class KeyEvent : public IEvent
{
public:
	KeyEvent(sf::Keyboard::Key key, bool isDown)
		: m_key(key)
		, m_isDown(isDown)
	{
	}

	inline sf::Keyboard::Key getKey() const
	{
		return m_key;
	}

	inline bool getIsDown() const
	{
		return m_isDown;
	}
private:
	sf::Keyboard::Key m_key;
	bool m_isDown;
};