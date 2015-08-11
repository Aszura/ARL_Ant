/* 
AISystem.h - Simon Dobersberger 
AI system for calculating the AI of the enemies.
*/
#pragma once

#include "BaseSystem.h"
#include <map>
#include <glm\glm.hpp>
#include <vector>

class EventManager;
class BaseComponent;
class EnemyComponent;
class ComponentDataModel;
class FieldComponent;
class IEvent;

class AISystem : public BaseSystem
{
public:
	AISystem(EventManager* eventMgr, ComponentDataModel* dataModel);
	~AISystem();

	void Start();
	void Update(double elapsed);
	void End();

	void EventListener(IEvent* eventData);
private:
	void CalculatePath();
	FieldComponent* GetNextField(FieldComponent* currentField, const std::vector<FieldComponent*>& walkedPath, float alpha, float beta);

	EventManager* m_eventManager;
	ComponentDataModel* m_dataModel;
	std::map<unsigned int, BaseComponent*>* m_transformComponents;
	std::map<unsigned int, BaseComponent*>* m_fieldComponents;
	std::map<unsigned int, BaseComponent*>* m_spriteComponents;

	FieldComponent* m_start;
	FieldComponent* m_goal;
};