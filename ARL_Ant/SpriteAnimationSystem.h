/* 
SpriteAnimationSystem.h - Simon Dobersberger 
Sprite animation system for animating sprites.
*/
#pragma once

#include "BaseSystem.h"
#include <map>
#include <vector>
#include <glm\glm.hpp>

class EventManager;
class BaseComponent;
class ComponentDataModel;
class IEvent;

class SpriteAnimationSystem : public BaseSystem
{
public:
	SpriteAnimationSystem(EventManager* eventMgr, ComponentDataModel* dataModel);
	~SpriteAnimationSystem();

	void Start();
	void Update(double elapsed);
	void End();

	void EventListener(IEvent* eventData);
private:
	EventManager* m_eventManager;
	ComponentDataModel* m_dataModel;
	std::map<unsigned int, BaseComponent*>* m_spriteComponents;
	std::map<unsigned int, BaseComponent*>* m_spriteAnimationComponents;
};