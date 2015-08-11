#pragma once

#include "BaseComponent.h"
#include <glm\glm.hpp>

class SpriteAnimationComponent : public BaseComponent
{
public:
	SpriteAnimationComponent(unsigned int entityId, unsigned int frames = 1, float animationSpeed = 1.0f)
		: BaseComponent(entityId)
		, m_frames(frames)
		, m_currentFrame(0)
		, m_startFrame(0)
		, m_endFrame(frames-1)
		, m_animationSpeed(animationSpeed)
		, m_frameBetween(0.0f)
		, m_isPlaying(false)
	{
	}

	SpriteAnimationComponent(unsigned int entityId, unsigned int startFrame, unsigned int endFrame, unsigned int frames = 1, float animationSpeed = 1.0f)
		: BaseComponent(entityId)
		, m_frames(frames)
		, m_currentFrame(0)
		, m_startFrame(startFrame)
		, m_endFrame(endFrame)
		, m_animationSpeed(animationSpeed)
		, m_frameBetween(0.0f)
		, m_isPlaying(false)
	{
	}

	inline unsigned int getFrames() const
	{
		return m_frames;
	}
	inline unsigned int getCurrentFrame() const
	{
		return m_currentFrame;
	}
	inline unsigned int getStartFrame() const
	{
		return m_startFrame;
	}
	inline unsigned int getEndFrame() const
	{
		return m_endFrame;
	}
	inline float getAnimationSpeed() const
	{
		return m_animationSpeed;
	}
	inline float getFrameBetween() const
	{
		return m_frameBetween;
	}
	inline bool getIsPlaying() const
	{
		return m_isPlaying;
	}

	inline void setFrames(unsigned int frames)
	{
		m_frames = frames;
	}
	inline void setCurrentFrame(unsigned int currentFrame)
	{
		m_currentFrame = currentFrame;
	}
	inline void setStartFrame(unsigned int startFrame)
	{
		m_startFrame = startFrame;
	}
	inline void setEndFrame(unsigned int endFrame)
	{
		m_endFrame = endFrame;
	}
	inline void setAnimationSpeed(float animationSpeed)
	{
		m_animationSpeed = animationSpeed;
	}
	inline void setFrameBetween(float frameBetween)
	{
		m_frameBetween = frameBetween;
	}
	inline void setIsPlaying(bool isPlaying)
	{
		m_isPlaying = isPlaying;
	}
private:
	unsigned int m_frames;
	unsigned int m_currentFrame;
	unsigned int m_startFrame;
	unsigned int m_endFrame;
	float m_animationSpeed;
	float m_frameBetween;
	bool m_isPlaying;
};