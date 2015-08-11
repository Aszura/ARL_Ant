#pragma once

#include <GL\glew.h>
#include <SFML\OpenGL.hpp>
#include <glm\glm.hpp>
#include <map>
#include <vector>
#include <string>

class Shader;
class Texture;

class SpriteBatch
{
public:
	SpriteBatch(unsigned int uBlockIndexGlobalMatrices, std::string shader = "SpriteShader");
	~SpriteBatch();
	void Begin();
	void End();
	void Draw(Texture* texture, glm::mat4 modelMatrix, glm::vec4 color);
	void Draw(Texture* texture, glm::mat4 modelMatrix, glm::vec4 textureBounds, glm::vec4 color);
private:
	struct SpriteData
	{
		SpriteData(const glm::mat4& modelMatrix, const glm::vec4& textureBounds, const glm::vec4& color)
			: m_modelMatrix(modelMatrix)
			, m_textureBounds(textureBounds)
			, m_color(color)
		{
		}

		glm::vec4 m_textureBounds;
		glm::vec4 m_color;
		glm::mat4 m_modelMatrix;
	};

	Shader* m_shader;
	GLuint m_drawVbo;
	GLuint m_quadVbo;
	GLuint m_vao;
	unsigned int m_spriteNum;

	std::map<Texture*, std::vector<SpriteData>> m_batchData;
};