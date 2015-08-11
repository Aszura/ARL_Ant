/* 
RenderGLSystem.h - Simon Dobersberger 
Render system for OpenGL context.
*/
#pragma once

#include <map>
#include "BaseSystem.h"
#include <glm\glm.hpp>
#include <GL\glew.h>
#include <SFML\Graphics.hpp>
#include <SFML\OpenGL.hpp>

class EventManager;
class BaseComponent;
class ComponentDataModel;
class SpriteBatch;
class PlayerComponent;

namespace sf
{
	class Window;
};

class RenderGLSystem : public BaseSystem
{
public:
	RenderGLSystem(EventManager* eventMgr, ComponentDataModel* dataModel);
	~RenderGLSystem();

	void Start();
	void Update(double elapsed);
	void End();

	void EventListener(IEvent* eventData);
private:
	std::map<unsigned int, BaseComponent*>* m_spriteComponents;
	std::map<unsigned int, BaseComponent*>* m_transformComponents;
	EventManager* m_eventManager;
	ComponentDataModel* m_dataModel;
	sf::RenderWindow* m_window;
	SpriteBatch* m_spriteBatch;

	GLuint m_globalMatricesUbo;
	const GLuint m_globalMatricesBlockBinding;

	glm::mat4 m_projectionMatrix;
	glm::mat4 m_viewMatrix;

	//Frame counter
	double m_frameTimeCounter;
	int m_frames;
	sf::Font m_guiFont;
	sf::Text m_guiMessage;
};