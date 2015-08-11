//#include "EventManager.h"
//#include "IEvent.h"
//#include "IEventListener.h"
//#include "EventType.h"
//#include <algorithm>
//
//void EventManager::FireEvent(IEvent* eventData)
//{
//	EventType type = eventData->getEventType();
//	for(unsigned int i = 0; i < m_listenerDict[type].size(); ++i)
//	{
//		m_listenerDict[type][i]->EventListener(eventData);
//	}
//}
//
//void EventManager::RegisterListener(EventType eventType, IEventListener* listener)
//{
//	m_listenerDict[eventType].push_back(listener);
//}
//
//void EventManager::RemoveListener(EventType eventType, IEventListener* listener)
//{
//	m_listenerDict[eventType].erase(
//		std::remove(m_listenerDict[eventType].begin(), m_listenerDict[eventType].end(), listener), 
//		m_listenerDict[eventType].end());
//}