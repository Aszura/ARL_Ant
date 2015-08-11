/* 
InputSystem.h - Simon Dobersberger 
Input system for input events.
*/
#pragma once

#include "BaseSystem.h"
#include <glm\glm.hpp>

class EventManager;
class BaseComponent;
class ComponentDataModel;
class IEvent;

class InputSystem : public BaseSystem
{
public:
	InputSystem(EventManager* eventMgr, ComponentDataModel* dataModel);
	~InputSystem();

	void Start();
	void Update(double elapsed);
	void End();

	void EventListener(IEvent* eventData);
private:
	EventManager* m_eventManager;
	ComponentDataModel* m_dataModel;
	bool* m_keysPressed;
	glm::ivec2 m_currentMousePosition;
	glm::ivec2 m_screenSize;
};