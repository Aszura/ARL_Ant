#pragma once

#include "Material.h"

class Sprite
{
public:
	Sprite(Material* material, glm::vec2 position)
		: m_material(material)
	{
	}

	~Sprite()
	{
		delete m_material;
	}

	Material* const getMaterial() const
	{
		return m_material;
	}

	void setMaterial(Material* material)
	{
		m_material = material;
	}
private:
	Material* m_material;
};