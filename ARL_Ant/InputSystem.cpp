#include "InputSystem.h"
#include "EventManager.h"
#include "KeyEvent.h"
#include "MousePositionEvent.h"
#include "ResizeScreenEvent.h"
#include <SFML\Window.hpp>

InputSystem::InputSystem(EventManager* eventMgr, ComponentDataModel* dataModel)
	: BaseSystem()
	, m_eventManager(eventMgr)
	, m_dataModel(dataModel)
	, m_keysPressed(new bool[sf::Keyboard::KeyCount])
{
}

InputSystem::~InputSystem()
{
	delete [] m_keysPressed;
}

void InputSystem::Start()
{
	for(unsigned int i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		m_keysPressed[i] = false;
	}

	m_eventManager->RegisterListener<ResizeScreenEvent>(this);
}

void InputSystem::Update(double elapsed)
{
	//Process keys
	for(unsigned int i = 0; i < sf::Keyboard::KeyCount; ++i)
	{
		sf::Keyboard::Key key = static_cast<sf::Keyboard::Key>(i);
		if(m_keysPressed[i] != sf::Keyboard::isKeyPressed(key))
		{
			m_eventManager->FireEvent<KeyEvent>(new KeyEvent(key, !m_keysPressed[i]));
			m_keysPressed[i] = !m_keysPressed[i];
		}
	}
}

void InputSystem::End()
{
	m_eventManager->RemoveListener<ResizeScreenEvent>(this);
}

void InputSystem::EventListener(IEvent* eventData)
{
	if(ResizeScreenEvent* event = dynamic_cast<ResizeScreenEvent*>(eventData))
	{
		m_screenSize = event->getScreenSize();
		sf::Mouse::setPosition(sf::Vector2i(m_screenSize.x/2, m_screenSize.y/2));
	}
}