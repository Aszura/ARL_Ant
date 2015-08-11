#pragma once

#include "BaseComponent.h"
#include <glm\glm.hpp>
#include <glm\ext.hpp>

class TransformComponent : public BaseComponent
{
public:
	TransformComponent(unsigned int entityId) 
		: BaseComponent(entityId)
		, m_position(glm::vec3(0.0f))
		, m_rotation(0.0f)
		, m_scale(glm::vec2(1.0f))
		, m_center(glm::vec2(0.0f))
	{
		RecalculateModelMatrix();
	}
	TransformComponent(unsigned int entityId, glm::vec3 position) 
		: BaseComponent(entityId)
		, m_position(position)
		, m_rotation(0.0f)
		, m_scale(glm::vec2(1.0f))
		, m_center(glm::vec2(0.0f))
	{
		RecalculateModelMatrix();
	}
	TransformComponent(unsigned int entityId, glm::vec3 position, float rotation, glm::vec2 scale, glm::vec2 center = glm::vec2(0.0f)) 
		: BaseComponent(entityId)
		, m_position(position)
		, m_rotation(rotation)
		, m_scale(scale)
		, m_center(center)
	{
		RecalculateModelMatrix();
	}
	~TransformComponent()
	{
	}

	inline const glm::vec3& getPosition() const
	{
		return m_position;
	}
	inline float getRotation() const
	{
		return m_rotation;
	}
	inline const glm::vec2& getScale() const
	{
		return m_scale;
	}
	inline const glm::vec2& getCenter() const
	{
		return m_center;
	}
	inline const glm::mat4& getModelMatrix() const
	{
		return m_modelMatrix;
	}

	inline void setPosition(const glm::vec3& position)
	{
		m_position = position;
		RecalculateModelMatrix();
	}
	inline void setRotation(float rotation)
	{
		m_rotation = rotation;
		RecalculateModelMatrix();
	}
	inline void setScale(const glm::vec2& scale)
	{
		m_scale = scale;
		RecalculateModelMatrix();
	}
	inline void setCenter(const glm::vec2& center)
	{
		m_center = center;
		RecalculateModelMatrix();
	}
	inline void setTransform(const glm::vec3& position, float rotation, const glm::vec2& scale)
	{
		m_position = position;
		m_rotation = rotation;
		m_scale = scale;
		RecalculateModelMatrix();
	}
private:
	void RecalculateModelMatrix()
	{
		m_modelMatrix = glm::mat4(1.0f);
		m_modelMatrix = glm::scale(m_modelMatrix, glm::vec3(m_scale.x, m_scale.y, 1.0f));
		m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(m_center.x, m_center.y, 0.0f));
		m_modelMatrix = glm::rotate(m_modelMatrix, m_rotation, 0.0f, 0.0f, 1.0f);
		m_modelMatrix = glm::translate(m_modelMatrix, m_position);
		//m_modelMatrix = glm::translate(m_modelMatrix, glm::vec3(-m_center.x, -m_center.y, 0.0f));
	}

	glm::vec3 m_position;
	float m_rotation;
	glm::vec2 m_scale;
	glm::vec2 m_center;

	glm::mat4 m_modelMatrix;
};