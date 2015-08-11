#pragma once

#include "BaseComponent.h"
#include <glm\glm.hpp>

class EnemyComponent : public BaseComponent
{
public:
	EnemyComponent(unsigned int entityId, glm::ivec2 startCoordinates, glm::ivec2 m_targetCoordinates)
		: BaseComponent(entityId)
		, m_coordinates(startCoordinates)
		, m_startCoordinates(startCoordinates)
		, m_targetCoordinates(m_targetCoordinates)
	{
	}

	//Getter
	inline const glm::ivec2& getCoordinates() const
	{
		return m_coordinates;
	}
	inline const glm::ivec2& getNextCoordinates() const
	{
		return m_nextCoordinates;
	}
	inline const glm::ivec2& getStartCoordinates() const
	{
		return m_startCoordinates;
	}
	inline const glm::ivec2& getTargetCoordinates() const
	{
		return m_targetCoordinates;
	}

	//Setter
	void setCoordinates(const glm::ivec2& coordinates)
	{
		m_coordinates = coordinates;
	}
	void setNextCoordinates(const glm::ivec2& nextCoordinates)
	{
		m_nextCoordinates = nextCoordinates;
	}
	void setStartCoordinates(const glm::ivec2& startCoordinates)
	{
		m_startCoordinates = startCoordinates;
	}
	void setTargetCoordinates(const glm::ivec2& argetCoordinates)
	{
		m_targetCoordinates = argetCoordinates;
	}
private:
	glm::ivec2 m_coordinates;
	glm::ivec2 m_nextCoordinates;
	glm::ivec2 m_startCoordinates;
	glm::ivec2 m_targetCoordinates;
};