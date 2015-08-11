#include "SpriteAnimationSystem.h"
#include "EventManager.h"
#include "SpriteAnimationComponent.h"
#include "SpriteComponent.h"
#include "ComponentDataModel.h"
#include "StartSpriteAnimationEvent.h"
#include "StopSpriteAnimationEvent.h"

#include <iostream>

SpriteAnimationSystem::SpriteAnimationSystem(EventManager* eventMgr, ComponentDataModel* dataModel)
	: BaseSystem()
	, m_eventManager(eventMgr)
	, m_dataModel(dataModel)
{
}

SpriteAnimationSystem::~SpriteAnimationSystem()
{
}

void SpriteAnimationSystem::Start()
{
	m_spriteComponents = m_dataModel->getComponents<SpriteComponent>();
	m_spriteAnimationComponents = m_dataModel->getComponents<SpriteAnimationComponent>();

	m_eventManager->RegisterListener<StartSpriteAnimationEvent>(this);
	m_eventManager->RegisterListener<StopSpriteAnimationEvent>(this);
}

void SpriteAnimationSystem::Update(double elapsed)
{
	for(std::map<unsigned int, BaseComponent*>::iterator it = m_spriteAnimationComponents->begin(); it != m_spriteAnimationComponents->end(); ++it)
	{
		SpriteAnimationComponent* spriteAnimation = static_cast<SpriteAnimationComponent*>(it->second);
		if(spriteAnimation->getIsPlaying())
		{
			if(m_spriteComponents->find(spriteAnimation->getEntityId()) != m_spriteComponents->end())
			{
				SpriteComponent* sprite = static_cast<SpriteComponent*>((*m_spriteComponents)[spriteAnimation->getEntityId()]);
				
				float t = spriteAnimation->getFrameBetween();
				if(t >= 1.0f)
				{
					unsigned int newFrame = spriteAnimation->getCurrentFrame()+1;
					if(newFrame > spriteAnimation->getEndFrame())
					{
						newFrame = spriteAnimation->getStartFrame();
					}
					spriteAnimation->setCurrentFrame(newFrame);
					spriteAnimation->setFrameBetween(0.0f);
					t = 0.0f;

					glm::vec4 textureBounds = sprite->getTextureBounds();
					glm::ivec2 texSize = sprite->getTexture()->getSize();
					textureBounds.x = static_cast<float>(static_cast<unsigned int>(newFrame * textureBounds.z) % texSize.x);
					textureBounds.y = glm::floor((newFrame * textureBounds.z) / texSize.x);
					sprite->setTextureBounds(textureBounds);
					//std::cout << "Frame: " << newFrame << std::endl;
				}
				spriteAnimation->setFrameBetween(t + static_cast<float>(elapsed) * spriteAnimation->getAnimationSpeed());
			}
		}
	}
}

void SpriteAnimationSystem::End()
{
	m_eventManager->RemoveListener<StartSpriteAnimationEvent>(this);
	m_eventManager->RemoveListener<StopSpriteAnimationEvent>(this);
}

void SpriteAnimationSystem::EventListener(IEvent* eventData)
{
	if(StartSpriteAnimationEvent* event = dynamic_cast<StartSpriteAnimationEvent*>(eventData))
	{
		if(m_spriteAnimationComponents->find(event->getEntityId()) != m_spriteAnimationComponents->end())
		{
			//std::cout << "Start animation \n";
			SpriteAnimationComponent* spriteAnimation = static_cast<SpriteAnimationComponent*>((*m_spriteAnimationComponents)[event->getEntityId()]);
			if(!spriteAnimation->getIsPlaying())
			{
				spriteAnimation->setIsPlaying(true);
				spriteAnimation->setCurrentFrame(spriteAnimation->getStartFrame());
			}
		}
	}
	else if(StopSpriteAnimationEvent* event = dynamic_cast<StopSpriteAnimationEvent*>(eventData))
	{
		if(m_spriteAnimationComponents->find(event->getEntityId()) != m_spriteAnimationComponents->end())
		{
			SpriteAnimationComponent* spriteAnimation = static_cast<SpriteAnimationComponent*>((*m_spriteAnimationComponents)[event->getEntityId()]);
			if(spriteAnimation->getIsPlaying())
			{
				spriteAnimation->setIsPlaying(false);
				spriteAnimation->setFrameBetween(0.0f);
			}
		}
	}
}