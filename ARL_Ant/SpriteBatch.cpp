#include "SpriteBatch.h"
#include "SpriteComponent.h"
#include "Shader.h"
#include "Texture.h"

#include <glm\ext.hpp>

SpriteBatch::SpriteBatch(unsigned int uBlockIndexGlobalMatrices, std::string shader)
	: m_vao(0)
	, m_drawVbo(0)
{
	glGenVertexArrays(1, &m_vao);
	glBindVertexArray(m_vao);

	glGenBuffers(1, &m_drawVbo);
	glGenBuffers(1, &m_quadVbo);
	glBindBuffer(GL_ARRAY_BUFFER, m_quadVbo);

	glm::vec2 quad[4];
	quad[0] = glm::vec2(0.0f, 0.0f);
	quad[1] = glm::vec2(1.0f, 0.0f);
	quad[2] = glm::vec2(0.0f, 1.0f);
	quad[3] = glm::vec2(1.0f, 1.0f);

	glBufferData(GL_ARRAY_BUFFER, sizeof(glm::vec2)*4, quad, GL_STATIC_DRAW);
	glVertexAttribPointer(0, 2, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(0);

	glBindBuffer(GL_ARRAY_BUFFER, m_drawVbo);
	unsigned int divisor = 1;

	//Texture Bounds
	glVertexAttribPointer(1, 4, GL_FLOAT, GL_FALSE, 0, 0);
	glEnableVertexAttribArray(1);
	glVertexAttribDivisor(1, divisor);

	//Color
	glVertexAttribPointer(2, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(sizeof(glm::vec4)));
	glEnableVertexAttribArray(2);
	glVertexAttribDivisor(2, divisor);

	//Matrix
	glVertexAttribPointer(3, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(sizeof(glm::vec4)*2));
	glEnableVertexAttribArray(3);
	glVertexAttribDivisor(3, divisor);
	glVertexAttribPointer(4, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(sizeof(glm::vec4)*3));
	glEnableVertexAttribArray(4);
	glVertexAttribDivisor(4, divisor);
	glVertexAttribPointer(5, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(sizeof(glm::vec4)*4));
	glEnableVertexAttribArray(5);
	glVertexAttribDivisor(5, divisor);
	glVertexAttribPointer(6, 4, GL_FLOAT, GL_FALSE, 0, reinterpret_cast<GLvoid*>(sizeof(glm::vec4)*5));
	glEnableVertexAttribArray(6);
	glVertexAttribDivisor(6, divisor);

	glBindVertexArray(0);
	glBindBuffer(GL_ARRAY_BUFFER, 0);

	string vertexShader = shader + ".vert";
	string fragmentShader = shader + ".frag";
	m_shader = new Shader(vertexShader.c_str(), fragmentShader.c_str());

	m_shader->LinkProg();
	m_shader->StoreUniformLocations();
	glUniformBlockBinding(m_shader->id(), m_shader->getUBlockGlobalMatricesIndex(), uBlockIndexGlobalMatrices);

#ifdef _DEBUG
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) 
	{
		cerr << "OpenGL error: " << err << endl;
	}
#endif
}

SpriteBatch::~SpriteBatch()
{
	glDeleteBuffers(1, &m_drawVbo);
	glDeleteBuffers(1, &m_quadVbo);
	glDeleteVertexArrays(1, &m_vao);
	delete m_shader;
}

void SpriteBatch::Begin()
{
	m_spriteNum = 0;
}

void SpriteBatch::End()
{
	glBindBuffer(GL_ARRAY_BUFFER, m_drawVbo);
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteData) * m_spriteNum, NULL, GL_STREAM_DRAW);
	m_shader->bind();

	for(std::map<Texture*, std::vector<SpriteData>>::iterator it = m_batchData.begin(); it != m_batchData.end(); ++it)
	{
		unsigned int numInstances = it->second.size();
		
		for(unsigned int i = 0; i < numInstances; ++i)
		{
			glBufferSubData(GL_ARRAY_BUFFER, i*sizeof(SpriteData), sizeof(SpriteData), reinterpret_cast<GLvoid*>(&it->second[i]));
		}
		
		Texture* texture = it->first;

		glActiveTexture(GL_TEXTURE0);
		glBindTexture(GL_TEXTURE_2D, texture->getTextureId());
		glUniform1i(m_shader->getColorMapLocation(), 0);
		glUniform2ui(m_shader->getTexSizeLocation(), texture->getSize().x, texture->getSize().y);

		glBindVertexArray(m_vao);
		glDrawArraysInstanced(GL_TRIANGLE_STRIP, 0, 4, numInstances*6);
		glBindVertexArray(0);
	}

	m_batchData.clear();
	m_shader->unbind();
	glBufferData(GL_ARRAY_BUFFER, sizeof(SpriteData) * m_spriteNum, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void SpriteBatch::Draw(Texture* texture, glm::mat4 modelMatrix, glm::vec4 color)
{
	Draw(texture, modelMatrix, glm::vec4(0.0f, 0.0f, static_cast<float>(texture->getSize().x), static_cast<float>(texture->getSize().y)), color);
}

void SpriteBatch::Draw(Texture* texture, glm::mat4 modelMatrix, glm::vec4 textureBounds, glm::vec4 color)
{
	m_batchData[texture].push_back(SpriteData(modelMatrix, textureBounds, color));
	++m_spriteNum;
}