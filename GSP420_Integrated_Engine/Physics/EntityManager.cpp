#include "EntityManager.h"
#include "EntityPropertyManager.h"

EntityManager::EntityManager()
{
	m_NextID = 0;
}

EntityManager::~EntityManager()
{
}

void EntityManager::Clear()
{
	m_Items.clear();
}

int EntityManager::AddItem(int type, int *id_OUT)
{
	pair<int, EntityData> newitem;
	newitem.second.Type = type;
	newitem.first = m_NextID;
	if(id_OUT != 0)
		*id_OUT = m_NextID;
	m_Items.push_back(newitem);
	return m_NextID++;
}

bool EntityManager::RemoveItem(int id)
{
	for(int i = 0 ;i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == id)
		{
			m_Items.erase(m_Items.begin() + i);
			return true;
		}
	}
	return false;
}

void EntityManager::SetValue(int *id, int type, void *value)
{
	for(int i = 0; i < m_Items.size(); ++i)
	{
		if(m_Items[i].first == *id)
		{
			m_Items[i].second.SetData(type, value);
		}
	}
}