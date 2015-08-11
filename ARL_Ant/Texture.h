#pragma once

#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <glm\glm.hpp>

class Texture{
public:
	Texture(const char* texturePath);
	~Texture();

	inline void setTextureId(GLuint textureId)
	{
		m_textureId = textureId;
	}
	inline glm::ivec2 getSize() const
	{
		return m_size;
	}
	inline GLuint getTextureId() const
	{
		return m_textureId;
	}
private:
	glm::ivec2 m_size;
	char* m_pixels;
	GLuint m_textureId;
};