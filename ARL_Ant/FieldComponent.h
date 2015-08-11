/* 
FieldComponent.h - Simon Dobersberger 
Field component for hexagonal board and pathfinding.
*/
#pragma once

#include "BaseComponent.h"
#include <glm\glm.hpp>
#include <vector>

class FieldComponent : public BaseComponent
{
public:
	FieldComponent(unsigned int entityId)
		: BaseComponent(entityId)
		, m_currentPheromoneStrength(0)
		, m_newPheromoneStrength(0)
		, m_isStart(false)
		, m_isGoal(false)
		, m_isPassable(true)
	{
	}

	//Getter
	inline float getCurrentPheromoneStrength() const
	{
		return m_currentPheromoneStrength;
	}
	inline float getNewPheromoneStrength() const
	{
		return m_newPheromoneStrength;
	}
	inline const glm::ivec2& getCoordinates() const
	{
		return m_coordinates;
	}
	inline std::vector<FieldComponent*>& getNeighbours()
	{
		return m_neighbours;
	}
	inline bool getIsPassable() const
	{
		return m_isPassable;
	}
	inline bool getIsStart() const
	{
		return m_isStart;
	}
	inline bool getIsGoal() const
	{
		return m_isGoal;
	}

	//Setter
	void setCurrentPheromoneStrength(float currentPheromoneStrength)
	{
		m_currentPheromoneStrength = currentPheromoneStrength;
	}
	void setNewPheromoneStrength(float newPheromoneStrength)
	{
		m_newPheromoneStrength = newPheromoneStrength;
	}
	void setCoordinates(const glm::ivec2& coordinates)
	{
		m_coordinates = coordinates;
	}
	void setIsPassable(bool isPassable)
	{
		m_isPassable = isPassable;
	}
	void setIsStart(bool isStart)
	{
		m_isStart = isStart;
	}
	void setIsGoal(bool isGoal)
	{
		m_isGoal = isGoal;
	}
private:
	std::vector<FieldComponent*> m_neighbours;
	glm::ivec2 m_coordinates;
	float m_currentPheromoneStrength;
	float m_newPheromoneStrength;
	bool m_isPassable;
	bool m_isStart;
	bool m_isGoal;
};