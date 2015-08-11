/* 
BaseComponent.h - Simon Dobersberger 
Base component for data components.
*/
#pragma once

class BaseComponent
{
public:
	virtual ~BaseComponent() {}

	inline unsigned int getEntityId() const
	{
		return m_entityId;
	}
protected:
	BaseComponent(unsigned int entityId)
	{
		m_entityId = entityId;
	}
	
private:
	unsigned int m_entityId;
};