#pragma once

#include <map>
#include <vector>
#include <typeinfo>
#include <typeindex>
#include <algorithm>
#include "IEventListener.h"
#include "IEvent.h"

class EventManager
{
public:
	template<class T>
	void FireEvent(T* eventData)
	{
		std::type_index type = std::type_index(typeid(T));
		
		for(unsigned int i = 0; i < m_listenerDict[type].size(); ++i)
		{
			m_listenerDict[type][i]->EventListener(eventData);
		}
		delete eventData;
	}

	template<class T>
	void QueueEvent(T* eventData)
	{
		std::type_index type = std::type_index(typeid(T));
		m_eventQueue[type].push_back(eventData);
		m_hasQueuedEvents = true;
	}

	void Update(double elapsed)
	{
		if(m_hasQueuedEvents)
		{
			for(std::map<std::type_index, std::vector<IEvent*>>::iterator it = m_eventQueue.begin(); it != m_eventQueue.end(); ++it)
			{
				for(unsigned int i = 0; i < it->second.size(); ++i)
				{
					for(unsigned int j = 0; j < m_listenerDict[it->first].size(); ++j)
					{
						m_listenerDict[it->first][j]->EventListener(it->second[i]);
					}
					delete it->second[i];
				}
				it->second.clear();
			}
			m_eventQueue.clear();
			m_hasQueuedEvents = false;
		}
	}

	template<class T>
	void RegisterListener(IEventListener* listener)
	{
		m_listenerDict[std::type_index(typeid(T))].push_back(listener);
	}

	template<class T>
	void RemoveListener(IEventListener* listener)
	{
		std::type_index type = std::type_index(typeid(T));
		m_listenerDict[type].erase(
			std::remove(m_listenerDict[type].begin(), m_listenerDict[type].end(), listener),
			m_listenerDict[type].end());
	}
private:
	std::map<std::type_index, std::vector<IEventListener*>> m_listenerDict;
	std::map<std::type_index, std::vector<IEvent*>> m_eventQueue;
	bool m_hasQueuedEvents;
};