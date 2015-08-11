#include "RenderGLSystem.h"
#include "BaseComponent.h"
#include "SpriteComponent.h"
#include "TransformComponent.h"
#include "ComponentDataModel.h"
#include "Shader.h"
#include "SpriteBatch.h"
#include "Constants.h"

#include "EventManager.h"
#include "IEvent.h"
#include "EventType.h"
#include "RegisterComponentEvent.h"
#include "ChangeGameStatusEvent.h"
#include "ResizeScreenEvent.h"
#include "MousePositionEvent.h"
#include "MouseClickEvent.h"
#include "ShowMessageEvent.h"

#include <iostream>
#include <SFML\Window.hpp>
#include <glm\ext.hpp>
#include <string>
#include <sstream>
#include <vector>

RenderGLSystem::RenderGLSystem(EventManager* eventMgr, ComponentDataModel* dataModel)
	: BaseSystem()
	, m_eventManager(eventMgr)
	, m_dataModel(dataModel)
	, m_frames(0)
	, m_frameTimeCounter(0.0)
	, m_globalMatricesBlockBinding(1)
{
	assert(eventMgr);
	assert(dataModel);

	//Set some window settings
	sf::ContextSettings settings;
	settings.antialiasingLevel = 0;
	settings.depthBits = 24;

	glm::uvec2 screenSize = glm::uvec2(SCREEN_SIZE_X, SCREEN_SIZE_Y);

	//Create window
	m_window = new sf::RenderWindow(sf::VideoMode(screenSize.x, screenSize.y), "KI - Pfadplanung", sf::Style::Default, settings);
	m_eventManager->QueueEvent<ResizeScreenEvent>(new ResizeScreenEvent(glm::ivec2(screenSize.x, screenSize.y)));

	//Enable Vsync
	m_window->setVerticalSyncEnabled(true);

	//Print settings
	settings = m_window->getSettings();
#ifdef _DEBUG
	std::cout << "depth bits:" << settings.depthBits << std::endl;
	std::cout << "stencil bits:" << settings.stencilBits << std::endl;
	std::cout << "antialiasing level:" << settings.antialiasingLevel << std::endl;
	std::cout << "version:" << settings.majorVersion << "." << settings.minorVersion << std::endl;
#endif

	//Load debug font
	m_guiFont.loadFromFile("fonts/arial.ttf");
	m_guiMessage = sf::Text("", m_guiFont, 30);

	//Initialize glew library
	GLenum err = glewInit();
	if (GLEW_OK != err)
	{
	  /* Problem: glewInit failed, something is seriously wrong. */
	  fprintf(stderr, "Error: %s\n", glewGetErrorString(err));
	}

	//Enable depth testing
	glEnable(GL_DEPTH_TEST);
	glDisable(GL_CULL_FACE);

	//Set clear color
	glClearColor( 0.0f, 0.0f, 0.0f, 0 );

	//Generate global matrices uniform buffer object
	glGenBuffers(1, &m_globalMatricesUbo);
	glBindBuffer(GL_UNIFORM_BUFFER, m_globalMatricesUbo);
	glBufferData(GL_UNIFORM_BUFFER, sizeof(glm::mat4) * 2, NULL, GL_STREAM_DRAW);
	glBindBuffer(GL_UNIFORM_BUFFER, 0);

	m_spriteBatch = new SpriteBatch(m_globalMatricesBlockBinding);
	glBindBufferRange(GL_UNIFORM_BUFFER, m_globalMatricesBlockBinding, m_globalMatricesUbo, 0, sizeof(glm::mat4) * 2);
}

RenderGLSystem::~RenderGLSystem()
{
	glDeleteBuffers(1, &m_globalMatricesUbo);
	delete m_spriteBatch;
	m_window->close();
}

void RenderGLSystem::Start()
{
	m_eventManager->RegisterListener<ShowMessageEvent>(this);
	m_spriteComponents = m_dataModel->getComponents<SpriteComponent>();
	m_transformComponents = m_dataModel->getComponents<TransformComponent>();
	
	//Buffer projection matrix to global matrices uniform buffer object
	//glm::mat4 m_projectionMatrix = glm::perspective<float>(60, static_cast<float>(m_window->getSize().x) / static_cast<float>(m_window->getSize().y), 0.1f, 100.0f);

	m_projectionMatrix = glm::ortho<float>(0.0f, static_cast<float>(m_window->getSize().x), static_cast<float>(m_window->getSize().y), 0.0f, -10.0f, 10.0f);
	m_viewMatrix = glm::lookAt(glm::vec3(0.0f, 0.0f, 0.0f), glm::vec3(0.0f, 0.0f, -1.0f), glm::vec3(0.0f, 1.0f, 0.0f));
	glBindBuffer(GL_UNIFORM_BUFFER, m_globalMatricesUbo);
	glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m_projectionMatrix[0][0]);
	glBufferSubData(GL_UNIFORM_BUFFER, sizeof(glm::mat4), sizeof(glm::mat4), &m_viewMatrix[0][0]);
    glBindBuffer(GL_UNIFORM_BUFFER, 0);
}

void RenderGLSystem::Update(double elapsed)
{
	// Handle SFML events
    sf::Event event;
    while (m_window->pollEvent(event))
    {
        if (event.type == sf::Event::Closed)
        {
            // end the program
            m_eventManager->FireEvent<ChangeGameStatusEvent>(new ChangeGameStatusEvent(GAME_STOPPED));
        }
        else if (event.type == sf::Event::Resized)
        {
            // adjust the viewport when the window is resized
            glViewport(0, 0, event.size.width, event.size.height);
			//glm::mat4 m_projectionMatrix = glm::perspective<float>(60, static_cast<float>(m_window->getSize().x) / static_cast<float>(m_window->getSize().y), 0.1f, 100.0f);
			m_projectionMatrix = glm::ortho<float>(0.0f, static_cast<float>(event.size.width), static_cast<float>(event.size.height), 0.0f, -10.0f, 10.0f);

			//Buffer projection matrix to global matrices uniform buffer object
			glBindBuffer(GL_UNIFORM_BUFFER, m_globalMatricesUbo);
			glBufferSubData(GL_UNIFORM_BUFFER, 0, sizeof(glm::mat4), &m_projectionMatrix[0][0]);
			glBindBuffer(GL_UNIFORM_BUFFER, 0);

			m_eventManager->FireEvent<ResizeScreenEvent>(new ResizeScreenEvent(glm::ivec2(event.size.width, event.size.height)));
        }
		else if(event.type == sf::Event::MouseMoved)
		{
			m_eventManager->FireEvent<MousePositionEvent>(new MousePositionEvent(glm::ivec2(event.mouseMove.x, event.mouseMove.y)));
		}
		else if (event.type == sf::Event::MouseButtonPressed)
		{
			m_eventManager->FireEvent<MouseClickEvent>(new MouseClickEvent(event.mouseButton.button));
		}
    }

	//Frame counter
	m_frames++;
	if((m_frameTimeCounter += elapsed) > 1)
	{
		std::ostringstream ss;
		ss << "KI - Pfadplanung - FPS: " << m_frames;
		std::string s(ss.str());
		//m_debugText = sf::Text(s, m_debugFont, 30);
		m_window->setTitle(s);
		m_frames = 0;
		m_frameTimeCounter = 0.0;
	}

    // Clear buffers
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

	m_spriteBatch->Begin();
    // Draw
	for(std::map<unsigned int, BaseComponent*>::iterator it = m_spriteComponents->begin(); it != m_spriteComponents->end(); ++it)
	{
		SpriteComponent* spriteComp = static_cast<SpriteComponent*>(it->second);
		unsigned int entityId = spriteComp->getEntityId();

		if(m_transformComponents->find(entityId) != m_transformComponents->end())
		{
			TransformComponent* transformComp = static_cast<TransformComponent*>((*m_transformComponents)[entityId]);
			m_spriteBatch->Draw(spriteComp->getTexture(), transformComp->getModelMatrix(), spriteComp->getTextureBounds(), spriteComp->getColor());
		}
	}
	m_spriteBatch->End();	

	//Draw debug text
	m_window->pushGLStates(); //save the current OpenGL state
	m_window->draw(m_guiMessage);
	m_window->popGLStates(); //restore the saved OpenGL state to the way it was before we drew the text

    // Swap back and front buffers
    m_window->display();

#ifdef _DEBUG
	GLenum err;
	while ((err = glGetError()) != GL_NO_ERROR) 
	{
		cerr << "OpenGL error: " << err << endl;
	}
#endif
}

void RenderGLSystem::End()
{
	m_eventManager->RemoveListener<ShowMessageEvent>(this);
}

void RenderGLSystem::EventListener(IEvent* eventData)
{
	if(ShowMessageEvent* event = dynamic_cast<ShowMessageEvent*>(eventData))
	{
		std::ostringstream ss;
		ss << event->getMessage();
		std::string s(ss.str());
		m_guiMessage = sf::Text(s, m_guiFont, 26);
		m_guiMessage.setPosition(10, 2);
		m_guiMessage.setColor(sf::Color::White);
	}
}