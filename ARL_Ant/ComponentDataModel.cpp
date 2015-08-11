#include "ComponentDataModel.h"
#include "IEvent.h"
#include "RegisterComponentEvent.h"
#include "DeleteEntityEvent.h"
#include "EventManager.h"

ComponentDataModel::ComponentDataModel(EventManager* eventMgr)
	: m_eventManager(eventMgr)
	, m_freeIdCounter(1)
{
	m_eventManager->RegisterListener<RegisterComponentEvent>(this);
	m_eventManager->RegisterListener<DeleteEntityEvent>(this);
}

ComponentDataModel::~ComponentDataModel()
{
	m_eventManager->RemoveListener<RegisterComponentEvent>(this);
	m_eventManager->RemoveListener<DeleteEntityEvent>(this);

	for(std::map<std::type_index, std::map<unsigned int, BaseComponent*>>::iterator ita = m_componentDict.begin(); ita != m_componentDict.end(); ++ita)
	{
		for(std::map<unsigned int, BaseComponent*>::iterator itb = ita->second.begin(); itb != ita->second.end(); ++itb)
		{
			delete itb->second;
		}
		ita->second.clear();
	}
	m_componentDict.clear();
}

void ComponentDataModel::EventListener(IEvent* eventData)
{
	if(RegisterComponentEvent* event = dynamic_cast<RegisterComponentEvent*>(eventData))
	{
		BaseComponent* component = event->getComponent();
		m_componentDict[std::type_index(typeid(*component))][component->getEntityId()] = component;
		m_usedEntityIds.insert(component->getEntityId());
	}
	else if(DeleteEntityEvent* event = dynamic_cast<DeleteEntityEvent*>(eventData))
	{
		for(std::map<std::type_index, std::map<unsigned int, BaseComponent*>>::iterator ita = m_componentDict.begin(); ita != m_componentDict.end(); ++ita)
		{
			if(ita->second.find(event->getEntityId()) != ita->second.end())
			{
				delete ita->second[event->getEntityId()];
				ita->second.erase(event->getEntityId());
				m_usedEntityIds.erase(event->getEntityId());
			}
		}
	}
}


unsigned int ComponentDataModel::getUnusedEntityId()
{
	std::set<unsigned int>::iterator it = m_usedEntityIds.lower_bound(m_freeIdCounter);
    while (it != m_usedEntityIds.end() && *it == m_freeIdCounter)
    {
        ++it;
        ++m_freeIdCounter;
    }
    return m_freeIdCounter++;
}