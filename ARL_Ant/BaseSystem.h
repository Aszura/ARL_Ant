/* 
BaseSystem.h - Simon Dobersberger 
Base system for computing components.
*/
#pragma once

#include "IEventListener.h"

class BaseSystem : public IEventListener
{
public:
	virtual ~BaseSystem() {}

	virtual void Start() {}
	virtual void Update(double elapsed) {}
	virtual void End() {}
protected:
	BaseSystem() {}
	
};