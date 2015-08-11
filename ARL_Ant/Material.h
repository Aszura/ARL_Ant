#pragma once

#include "Shader.h"
#include "Texture.h"
#include <GL\glew.h>
#include <SFML\OpenGL.hpp>


class Material
{
public:
	Material(Shader* shader)
		: m_shader(shader)
		, m_globalMatricesBlockIndex(0)
		, m_lightingBlockIndex(1)
	{
	}
	~Material()
	{

	}
	void setShader(Shader* shader)
	{
		m_shader = shader;
	}
	void setTexture(Texture* texture)
	{
		m_texture = texture;
	}
	void setGlobalMatricesBlockIndex(GLuint globalmatricesBlockIndex)
	{
		m_globalMatricesBlockIndex = globalmatricesBlockIndex;
	}
	void setLightingBlockIndex(GLuint lightingBlockIndex)
	{
		m_lightingBlockIndex = lightingBlockIndex;
	}
	Shader* getShader() const
	{
		return m_shader;
	}
	Texture* getTexture() const
	{
		return m_texture;
	}
	GLuint getGlobalMatricesBlockIndex() const
	{
		return m_globalMatricesBlockIndex;
	}
	GLuint getLightingBlockIndex() const
	{
		return m_lightingBlockIndex;
	}
private:
	Shader* m_shader;
	Texture* m_texture;
	GLuint m_globalMatricesBlockIndex;
	GLuint m_lightingBlockIndex;
};