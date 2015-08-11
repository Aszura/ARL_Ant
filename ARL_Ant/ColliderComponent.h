#pragma once

#include "BaseComponent.h"

class ColliderComponent : public BaseComponent
{
public:
	ColliderComponent(unsigned int entityId)
		: BaseComponent(entityId)
	{
	}
};