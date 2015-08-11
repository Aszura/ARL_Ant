#include "FieldSystem.h"
#include "EventManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "FieldComponent.h"
#include "EnemyComponent.h"
#include "ComponentDataModel.h"
#include "DirectionEnum.h"
#include "Constants.h"
#include "TextureLoader.h"
#include "RegisterComponentEvent.h"

#include "MousePositionEvent.h"
#include "MouseClickEvent.h"
#include "KeyEvent.h"
#include "ShowMessageEvent.h"
#include "StepEvent.h"
#include "StartSetEvent.h"
#include "GoalSetEvent.h"
#include "PathFinishedEvent.h"
#include "RecalculateFieldEvent.h"

#include <iostream>
#include <vector>

FieldSystem::FieldSystem(EventManager* eventMgr, ComponentDataModel* dataModel, TextureLoader* textureLoader)
	: BaseSystem()
	, m_eventManager(eventMgr)
	, m_dataModel(dataModel)
	, m_textureLoader(textureLoader)
	, m_gameMode(MODE_CHOOSE_START)
	, m_automaticModeEnabled(false)
{
}

FieldSystem::~FieldSystem()
{
}

void FieldSystem::Start()
{
	m_eventManager->RegisterListener<MousePositionEvent>(this);
	m_eventManager->RegisterListener<MouseClickEvent>(this);
	m_eventManager->RegisterListener<KeyEvent>(this);
	m_eventManager->RegisterListener<PathFinishedEvent>(this);
	m_eventManager->RegisterListener<RecalculateFieldEvent>(this);

	m_transformComponents = m_dataModel->getComponents<TransformComponent>();
	m_fieldComponents = m_dataModel->getComponents<FieldComponent>();
	m_spriteComponents = m_dataModel->getComponents<SpriteComponent>();

	CreateBoard();
	m_eventManager->QueueEvent<ShowMessageEvent>(new ShowMessageEvent("Please choose a starting point."));
}

void FieldSystem::CreateBoard()
{
	//Temporary board for building neighbourhood
	std::vector<std::vector<FieldComponent*>> fieldBoard;
	fieldBoard.resize(FIELDSIZE_Y);

	for(unsigned int y = 0; y < FIELDSIZE_Y; ++y)
	{
		fieldBoard[y].resize(FIELDSIZE_X);
		for(unsigned int x = 0; x < FIELDSIZE_X; ++x)
		{
			//Create field
			unsigned int entityId = m_dataModel->getUnusedEntityId();
			SpriteComponent* sprite = new SpriteComponent(entityId);
			sprite->setTexture(m_textureLoader->Load("resources/textures/hexagon.png"));
			int height = static_cast<int>(sprite->getTextureBounds().w);
			TransformComponent* hexagonTransform = new TransformComponent(entityId, glm::vec3(BOARD_OFFSET_X + x * FIELD_OFFSET_X, BOARD_OFFSET_Y + y  * height + (x % 2) * (height / 2), -5));
			hexagonTransform->setScale(glm::vec2(0.4f));
			FieldComponent* fieldComp = new FieldComponent(entityId);
			fieldComp->setCoordinates(glm::ivec2(x, y));
			m_eventManager->FireEvent<RegisterComponentEvent>(new RegisterComponentEvent(sprite));
			m_eventManager->FireEvent<RegisterComponentEvent>(new RegisterComponentEvent(hexagonTransform));
			m_eventManager->FireEvent<RegisterComponentEvent>(new RegisterComponentEvent(fieldComp));

			fieldBoard[y][x] = fieldComp;
			
			//Build neighbourhood
			if(y > 0)
			{
				fieldBoard[y-1][x]->getNeighbours().push_back(fieldComp);
				fieldComp->getNeighbours().push_back(fieldBoard[y-1][x]);
			}

			if(x > 0)
			{
				fieldBoard[y][x-1]->getNeighbours().push_back(fieldComp);
				fieldComp->getNeighbours().push_back(fieldBoard[y][x-1]);
			}

			if(x > 0 && y > 0 && x % 2 == 0)
			{
				fieldBoard[y-1][x-1]->getNeighbours().push_back(fieldComp);
				fieldComp->getNeighbours().push_back(fieldBoard[y-1][x-1]);
			}

			if(x < FIELDSIZE_X - 1 && y > 0 && x % 2 == 0)
			{
				fieldBoard[y-1][x+1]->getNeighbours().push_back(fieldComp);
				fieldComp->getNeighbours().push_back(fieldBoard[y-1][x+1]);
			}

		}
	}

	//Create hole/unpassable fields
	fieldBoard[7][9]->setIsPassable(false);
	fieldBoard[7][10]->setIsPassable(false);
	fieldBoard[7][11]->setIsPassable(false);
	fieldBoard[7][12]->setIsPassable(false);
	fieldBoard[7][13]->setIsPassable(false);
	RecalculateColors();
}

//Recalculate colors
void FieldSystem::RecalculateColors()
{
	for(std::map<unsigned int, BaseComponent*>::iterator it = m_fieldComponents->begin(); it != m_fieldComponents->end(); ++it)
	{
		FieldComponent* fieldComp = static_cast<FieldComponent*>(it->second);
		unsigned int entityId = fieldComp->getEntityId();

		SpriteComponent* spriteComp = static_cast<SpriteComponent*>((*m_spriteComponents)[entityId]);

		if(!fieldComp->getIsPassable())
		{
			spriteComp->setColor(glm::vec4(0.5,0.2,0.05,1));
			continue;
		}


		if(fieldComp->getIsStart())
		{
			spriteComp->setColor(glm::vec4(0,1,0,1));
		}
		else if(fieldComp->getIsGoal())
		{
			spriteComp->setColor(glm::vec4(0,0,1,1));
		}
		else
		{
			spriteComp->setColor(glm::vec4(1.0f/(fieldComp->getCurrentPheromoneStrength()+1)));
		}
	}
}

void FieldSystem::UpdatePheromones()
{
	static const float evaporation = 0.2f;

	for (std::map<unsigned int, BaseComponent*>::iterator it = m_fieldComponents->begin(); it != m_fieldComponents->end(); ++it)
	{
		FieldComponent* fieldComp = static_cast<FieldComponent*>(it->second);

		float strength = (1 - evaporation) * fieldComp->getCurrentPheromoneStrength() + fieldComp->getNewPheromoneStrength();
		fieldComp->setCurrentPheromoneStrength(strength);
		fieldComp->setNewPheromoneStrength(0);
	}
}

void FieldSystem::Update(double elapsed)
{
	if (m_automaticModeEnabled && m_gameMode == MODE_DONE)
	{
		m_eventManager->FireEvent<StepEvent>(new StepEvent());
		UpdatePheromones();
		RecalculateColors();
	}
}

void FieldSystem::End()
{
	m_eventManager->RemoveListener<MouseClickEvent>(this);
	m_eventManager->RemoveListener<KeyEvent>(this);
	m_eventManager->RemoveListener<PathFinishedEvent>(this);
	m_eventManager->RemoveListener<MousePositionEvent>(this);
	m_eventManager->RemoveListener<RecalculateFieldEvent>(this);
}

void FieldSystem::EventListener(IEvent* eventData)
{
	if(MousePositionEvent* event = dynamic_cast<MousePositionEvent*>(eventData))
	{
		glm::ivec2 mousePos = event->getMousePosition();
		m_pickedField = nullptr;

		//Simple mouse picking for field
		for(std::map<unsigned int, BaseComponent*>::iterator it = m_fieldComponents->begin(); it != m_fieldComponents->end(); ++it)
		{
			FieldComponent* fieldComp = static_cast<FieldComponent*>(it->second);
			unsigned int entityId = fieldComp->getEntityId();

			TransformComponent* transformComp = static_cast<TransformComponent*>((*m_transformComponents)[entityId]);
			SpriteComponent* spriteComp = static_cast<SpriteComponent*>((*m_spriteComponents)[entityId]);

			glm::vec4 bounds = glm::vec4(transformComp->getPosition().x * transformComp->getScale().x, transformComp->getPosition().y * transformComp->getScale().y, spriteComp->getTextureBounds().z * transformComp->getScale().x, spriteComp->getTextureBounds().w * transformComp->getScale().y);

			bool isPickedField = m_pickedField == nullptr && fieldComp->getIsPassable() && mousePos.x > bounds.x && mousePos.x < bounds.x + bounds.z && mousePos.y > bounds.y && mousePos.y < bounds.y + bounds.w;

			//Either showing start or goal to set
			switch(m_gameMode)
			{
			case MODE_CHOOSE_START:
				if(isPickedField)
				{
					fieldComp->setIsStart(true);
					m_pickedField = fieldComp;
				}
				else
				{
					fieldComp->setIsStart(false);
				}
				break;
			case MODE_CHOOSE_GOAL:
				if(isPickedField)
				{
					fieldComp->setIsGoal(true);
					m_pickedField = fieldComp;
				}
				else
				{
					fieldComp->setIsGoal(false);
				}
				break;
			case MODE_DONE:
				if (isPickedField)
				{
					m_pickedField = fieldComp;
				}
				break;
			default:
				break;
			}
		}
		RecalculateColors();
	}
	else if(MouseClickEvent* event = dynamic_cast<MouseClickEvent*>(eventData))
	{
		if (event->getButton() == sf::Mouse::Button::Left)
		{
			//Take enemy step/reculculate path or set start/goal
			if (m_gameMode == MODE_DONE)
			{
				m_eventManager->FireEvent<StepEvent>(new StepEvent());
				UpdatePheromones();
				RecalculateColors();
			}
			else
			{
				if (m_pickedField != nullptr)
				{
					switch (m_gameMode)
					{
					case MODE_CHOOSE_START:
						m_eventManager->FireEvent<StartSetEvent>(new StartSetEvent(m_pickedField));
						std::cout << "Start set" << std::endl;
						m_eventManager->FireEvent<ShowMessageEvent>(new ShowMessageEvent("Please choose a goal point."));
						m_gameMode = MODE_CHOOSE_GOAL;
						break;
					case MODE_CHOOSE_GOAL:
						m_eventManager->FireEvent<GoalSetEvent>(new GoalSetEvent(m_pickedField));
						std::cout << "Goal set" << std::endl;
						m_eventManager->FireEvent<ShowMessageEvent>(new ShowMessageEvent("Press mouse button for simulation step or 'A' for automatic mode."));
						m_gameMode = MODE_DONE;
						break;
					default:
						break;
					}
				}
			}
		}
		else if (event->getButton() == sf::Mouse::Button::Right)
		{
			if (m_pickedField != nullptr)
			{
				m_pickedField->setIsPassable(!m_pickedField->getIsPassable());
				m_pickedField->setCurrentPheromoneStrength(0);
				m_pickedField->setNewPheromoneStrength(0);
				RecalculateColors();
			}
		}
	}
	else if (KeyEvent* event = dynamic_cast<KeyEvent*>(eventData))
	{
		if (m_gameMode == MODE_DONE && event->getKey() == sf::Keyboard::A && event->getIsDown())
		{
			m_automaticModeEnabled = !m_automaticModeEnabled;
		}
	}
}