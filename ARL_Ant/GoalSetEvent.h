#pragma once

#include "IEvent.h"
#include "FieldComponent.h"

class GoalSetEvent : public IEvent
{
public:
	GoalSetEvent(FieldComponent* goal)
		: m_goal(goal)
	{
	}

	inline FieldComponent* getGoalField() const
	{
		return m_goal;
	}
private:
	FieldComponent* m_goal;
};