#pragma once

#include <iostream>
#include <vector>
#include "IEventListener.h"
#include <SFML\System.hpp>
#include <glm\glm.hpp>

class BaseSystem;
class EventManager;
class ComponentDataModel;
class IEvent;
class TextureLoader;
enum GameStatusEnum;

class Game : public IEventListener
{
public:
	Game();
	~Game();
	void Run();

	void EventListener(IEvent* eventData);
private:
	EventManager* m_eventManager;
	ComponentDataModel* m_dataModel;
	std::vector<BaseSystem*> m_systems;
	GameStatusEnum m_gameStatus;
	sf::Clock m_clock;
	TextureLoader* m_textureLoader;
};