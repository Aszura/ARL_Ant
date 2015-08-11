#include "AISystem.h"
#include "EventManager.h"
#include "TransformComponent.h"
#include "SpriteComponent.h"
#include "EnemyComponent.h"
#include "FieldComponent.h"
#include "ComponentDataModel.h"
#include "DirectionEnum.h"
#include "StepEvent.h"
#include "StartSetEvent.h"
#include "GoalSetEvent.h"
#include "PathFinishedEvent.h"
#include "RecalculateFieldEvent.h"
#include "Constants.h"

#include <iostream>
#include <vector>
#include <stack>
#include <algorithm>

AISystem::AISystem(EventManager* eventMgr, ComponentDataModel* dataModel)
	: BaseSystem()
	, m_eventManager(eventMgr)
	, m_dataModel(dataModel)
{
	assert(eventMgr);
	assert(dataModel);
}

AISystem::~AISystem()
{
}

void AISystem::Start()
{
	m_eventManager->RegisterListener<StepEvent>(this);
	m_eventManager->RegisterListener<StartSetEvent>(this);
	m_eventManager->RegisterListener<GoalSetEvent>(this);

	m_transformComponents = m_dataModel->getComponents<TransformComponent>();
	m_fieldComponents = m_dataModel->getComponents<FieldComponent>();
	m_spriteComponents = m_dataModel->getComponents<SpriteComponent>();
}

void AISystem::Update(double elapsed)
{
}

void AISystem::End()
{
	m_eventManager->RemoveListener<StepEvent>(this);
	m_eventManager->RemoveListener<StartSetEvent>(this);
	m_eventManager->RemoveListener<GoalSetEvent>(this);
}

void AISystem::EventListener(IEvent* eventData)
{
	assert(eventData);

	if(StartSetEvent* event = dynamic_cast<StartSetEvent*>(eventData))
	{
		m_start = event->getStartField();
	}
	else if(GoalSetEvent* event = dynamic_cast<GoalSetEvent*>(eventData))
	{
		m_goal = event->getGoalField();
	}
	else if(StepEvent* event = dynamic_cast<StepEvent*>(eventData))
	{
		m_eventManager->FireEvent<RecalculateFieldEvent>(new RecalculateFieldEvent());
		CalculatePath();
	}
}

//Calculate path via Ant Colony
void AISystem::CalculatePath()
{
	static const unsigned int antsNumber = 50;
	static const float alpha = 2.5f;
	static const float beta = 1.0f;
	static int round = 0;

	for (unsigned int ant = 0; ant < antsNumber; ++ant)
	{
		FieldComponent* currentField = m_start;
		std::vector<FieldComponent*> walkedPath;

		while (currentField != nullptr && currentField != m_goal)
		{
			walkedPath.push_back(currentField);
			currentField = GetNextField(currentField, walkedPath, alpha, beta);
		}

		if (currentField != nullptr)
		{
			int bestDistance = abs(m_start->getCoordinates().x - m_goal->getCoordinates().x) + abs(m_start->getCoordinates().y - m_goal->getCoordinates().y);
			float strengthPerField = static_cast<float>(bestDistance) / walkedPath.size();

			for (unsigned int i = 0; i < walkedPath.size(); ++i)
			{
				float newPheromoneStrength = walkedPath[i]->getNewPheromoneStrength() + strengthPerField;
				walkedPath[i]->setNewPheromoneStrength(newPheromoneStrength);
			}
		}
	}

	++round;
}

FieldComponent* AISystem::GetNextField(FieldComponent* currentField, const std::vector<FieldComponent*>& walkedPath, float alpha, float beta)
{
	unsigned int neighboursNum = currentField->getNeighbours().size();
	float sum = 0.0f;
	float* visitProb = new float[neighboursNum];

	for (unsigned int i = 0; i < neighboursNum; ++i)
	{
		FieldComponent* neighbour = currentField->getNeighbours()[i];

		if (neighbour->getIsPassable() && std::count(walkedPath.begin(), walkedPath.end(), neighbour) == 0)
		{
			//Manhatten distance
			int distance = abs(neighbour->getCoordinates().x - m_goal->getCoordinates().x) + abs(neighbour->getCoordinates().y - m_goal->getCoordinates().y);

			//Found food
			if (distance == 0)
			{
				return neighbour;
			}

			visitProb[i] = std::pow(1 + neighbour->getCurrentPheromoneStrength(), alpha) * std::pow(1.0f/1, beta);
			sum += visitProb[i];
		}
		else
		{
			visitProb[i] = 0;
		}
	}

	if (sum == 0.0f)
	{
		return nullptr;
	}

	//Roulette-wheel-selection
	float luckyNumber = ((float)rand() / (RAND_MAX)) * sum;
	unsigned int selected = 0;

	while (luckyNumber >= 0.0 && selected < neighboursNum) {
		luckyNumber -= visitProb[selected];
		selected++;
	}

	delete[] visitProb;
	return currentField->getNeighbours()[selected-1];
}
