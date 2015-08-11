/* 
FieldSystem.h - Simon Dobersberger 
Field system for calculating the costs for the fields.
*/
#pragma once

#include "BaseSystem.h"
#include "GameModeEnum.h"
#include <map>
#include <glm\glm.hpp>
#include <vector>

class EventManager;
class BaseComponent;
class ComponentDataModel;
class TextureLoader;
class FieldComponent;
class EnemyComponent;
class IEvent;

class FieldSystem : public BaseSystem
{
public:
	FieldSystem(EventManager* eventMgr, ComponentDataModel* dataModel, TextureLoader* textureLoader);
	~FieldSystem();

	void Start();
	void Update(double elapsed);
	void End();

	void EventListener(IEvent* eventData);
private:
	void CreateBoard();
	void UpdatePheromones();
	void RecalculateColors();

	std::map<unsigned int, BaseComponent*>* m_transformComponents;
	std::map<unsigned int, BaseComponent*>* m_fieldComponents;
	std::map<unsigned int, BaseComponent*>* m_spriteComponents;

	FieldComponent* m_pickedField;
	TextureLoader* m_textureLoader;
	EventManager* m_eventManager;
	ComponentDataModel* m_dataModel;
	GameModeEnum m_gameMode;
	bool m_automaticModeEnabled;
};