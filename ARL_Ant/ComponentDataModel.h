#pragma once

#include <map>
#include <set>
#include <typeinfo>
#include <typeindex>
#include "BaseComponent.h"
#include "IEventListener.h"

class IEvent;
class EventManager;

class ComponentDataModel : public IEventListener
{
public:
	ComponentDataModel(EventManager* eventMgr);
	~ComponentDataModel();
	void EventListener(IEvent* eventData);
	unsigned int getUnusedEntityId();

	template<class T>
	std::map<unsigned int, BaseComponent*>* getComponents()
	{
		return &m_componentDict[std::type_index(typeid(T))];
	}
private:
	std::map<std::type_index, std::map<unsigned int, BaseComponent*>> m_componentDict;
	EventManager* m_eventManager;
	std::set<unsigned int> m_usedEntityIds;
	unsigned int m_freeIdCounter;
};