#include "Game.h"
#include "EventManager.h"
#include "IEvent.h"
#include "RegisterComponentEvent.h"
#include "ChangeGameStatusEvent.h"
#include "KeyEvent.h"
#include "TextureLoader.h"
#include "Constants.h"

#include "ComponentDataModel.h"
#include "GameStatusEnum.h"

#include "BaseSystem.h"
#include "RenderGLSystem.h"
#include "InputSystem.h"
#include "SpriteAnimationSystem.h"
#include "AISystem.h"
#include "FieldSystem.h"

#include <fstream>
#include <string>
#include <iostream>

Game::Game()
	: m_eventManager(new EventManager())
	, m_dataModel(new ComponentDataModel(m_eventManager))
	, m_gameStatus(GAME_RUNNING)
	, m_textureLoader(new TextureLoader())
{
	m_eventManager->RegisterListener<ChangeGameStatusEvent>(this);
	m_eventManager->RegisterListener<KeyEvent>(this);

	//Create systems, in right update order
	m_systems.push_back(new FieldSystem(m_eventManager, m_dataModel, m_textureLoader));
	m_systems.push_back(new InputSystem(m_eventManager, m_dataModel));
	m_systems.push_back(new AISystem(m_eventManager, m_dataModel));
	m_systems.push_back(new SpriteAnimationSystem(m_eventManager, m_dataModel));
	m_systems.push_back(new RenderGLSystem(m_eventManager, m_dataModel));
}

Game::~Game()
{
	m_eventManager->RemoveListener<ChangeGameStatusEvent>(this);
	m_eventManager->RemoveListener<KeyEvent>(this);

	for(unsigned int i = 0; i < m_systems.size(); ++i)
	{
		delete m_systems[i];
	}
	m_systems.clear();

	delete m_dataModel;
	delete m_eventManager;
	delete m_textureLoader;
}

void Game::Run()
{
	for(unsigned int i = 0; i < m_systems.size(); ++i)
	{
		m_systems[i]->Start();
	}

	while(m_gameStatus != GAME_STOPPED)
	{
		sf::Time time = m_clock.restart();
		m_eventManager->Update(time.asSeconds());
		for(unsigned int i = 0; i < m_systems.size(); ++i)
		{
			m_systems[i]->Update(time.asSeconds());
		}
	}

	for(unsigned int i = 0; i < m_systems.size(); ++i)
	{
		m_systems[i]->End();
	}
}

void Game::EventListener(IEvent* eventData)
{
	if(typeid(*eventData) == typeid(ChangeGameStatusEvent))
	{
		ChangeGameStatusEvent* event = static_cast<ChangeGameStatusEvent*>(eventData);
		m_gameStatus = event->getNewStatus();
	}
	if(typeid(*eventData) == typeid(KeyEvent))
	{
		KeyEvent* event = static_cast<KeyEvent*>(eventData);
		if(event->getKey() == sf::Keyboard::Escape)
		{
			m_eventManager->FireEvent<ChangeGameStatusEvent>(new ChangeGameStatusEvent(GAME_STOPPED));
		}
	}
}