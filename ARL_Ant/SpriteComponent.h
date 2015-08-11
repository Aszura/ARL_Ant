#pragma once

#include "BaseComponent.h"
#include "Shader.h"
#include "Texture.h"
#include <glm\glm.hpp>

class SpriteComponent : public BaseComponent
{
public:
	SpriteComponent(unsigned int entityId)
		: BaseComponent(entityId)
		, m_color(glm::vec4(1.0f))
	{
	}

	inline Texture* getTexture() const
	{
		return m_texture;
	}
	inline const glm::vec4& getTextureBounds() const
	{
		return m_textureBounds;
	}
	inline const glm::vec4& getColor() const
	{
		return m_color;
	}

	inline void setTexture(Texture* texture)
	{
		m_texture = texture;
		if(m_textureBounds == glm::vec4(0.0f))
		{
			setTextureBounds(glm::vec4(0.0f, 0.0f, m_texture->getSize().x, m_texture->getSize().y));
		}
	}
	inline void setTextureBounds(const glm::vec4& textureBounds)
	{
		m_textureBounds = textureBounds;
	}
	inline void setColor(const glm::vec4& color)
	{
		m_color = color;
	}
private:
	Texture* m_texture;
	glm::vec4 m_textureBounds;
	glm::vec4 m_color;
};