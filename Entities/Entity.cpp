#include "Entity.h"
#include "EntityManager.h"
#include "Messenger.h"

namespace Tijss
{
	extern cMessenger Messenger;

	cEntity::cEntity
	(
		cEntityTemplate* entityTemplate,
		TEntityUID       UID,
		cEntityManager* entityManager,
		const string&    name /*=""*/
	)
	{
		m_Template = entityTemplate;
		m_UID = UID;
		m_Name = name;
		m_Manager = entityManager;
		m_Transform.isAnimated = false;
		m_Active = true;
	}

	// components

	void cEntity::AddComponent(cEntityComponent* component)
	{
		m_Components.insert(pair<TEntityUID, cEntityComponent*>(component->GetUID(), component));
	}

	cEntityComponent* cEntity::GetComponent(TEntityUID UID)
	{
		map<TEntityUID, cEntityComponent*>::iterator it = m_Components.find(UID);
		if (it != m_Components.end())
		{
			return (*it).second;
		}
		return 0;
	}

	cEntityComponent* cEntity::GetComponent(const string& type)
	{
		map<TEntityUID, cEntityComponent*>::iterator it = m_Components.begin();
		while (it != m_Components.end())
		{
			if ((*it).second->GetType() == type)
			{
				return (*it).second;
			}
			it++;
		}
		return 0;
	}

	bool cEntity::RemoveComponent(TEntityUID UID)
	{
		map<TEntityUID, cEntityComponent*>::iterator it = m_Components.find(UID);
		if (it != m_Components.end())
		{
			delete (*it).second;
			m_Components.erase(it);
			return true;
		}
		return false;
	}

	bool cEntity::RemoveComponent(const string& type)
	{
		map<TEntityUID, cEntityComponent*>::iterator it = m_Components.begin();
		while (it != m_Components.end())
		{
			if ((*it).second->GetType() == type)
			{
				delete (*it).second;
				m_Components.erase(it);
				return true;
			}
			it++;
		}
		return false;

	}

	void cEntity::RemoveAllComponents()
	{
		map<TEntityUID, cEntityComponent*>::iterator it = m_Components.begin();
		while (it != m_Components.end())
		{
			delete (*it).second;
			it++;
		}
		m_Components.clear();
	}

	bool cEntity::Update(float32 updateTime)
	{
		sMessage msg;
		std::map<TEntityUID, cEntityComponent*>::iterator iter = m_Components.begin();

		while (Messenger.FetchMsg(m_UID, &msg))
		{
			switch (msg.type)
			{
				//received destroy message
			case Msg_Die:
				return false;

			default:
				// redirect messanges to components
				while (iter != m_Components.end())
				{
					Messenger.SendMsg((*iter).first, msg);
					iter++;
				}
				iter = m_Components.begin();
			}
		}

		if (m_Active)
		{
			// update all componenets
			while (iter != m_Components.end())
			{
				if (!(*iter).second->Update(updateTime))
				{
					iter = m_Components.erase(iter);
				}
				else
				{
					iter++;
				}
			}
		}
		return true;
	}
	
	void Transform::Move(int x, int y)
	{
		prevPosition = position;
		position.x += x;
		position.y += y;
		if (layerMoved)
			*layerMoved = true;
	}

	void Transform::Move(sFloat2 vector)
	{
		prevPosition = position;
		position.x += vector.x;
		position.y += vector.y;
		if (layerMoved)
			*layerMoved = true;
	}

	void Transform::SetPosition(sFloat2 position)
	{
		prevPosition = position;
		this->position = position;
		if (layerMoved)
			*layerMoved = true;
	}

	void Transform::SetPosition(int x, int y)
	{
		prevPosition = position;
		position.x = x;
		position.y = y;
		if (layerMoved)
			*layerMoved = true;
	}

	void Transform::Rotate(float angle)
	{
		this->rotation += angle;
	}

	void Transform::SetRotation(float rotation)
	{
		this->rotation = rotation;
	}

	void Transform::Scale(float scalar)
	{
		scale.x += scalar;
		scale.y += scalar;
		if (layerMoved)
			*layerMoved = true;
	}

	void Transform::Scale(float x, float y)
	{
		scale.x += x;
		scale.y += y;
		if (layerMoved)
			*layerMoved = true;
	}

	void Transform::Scale(sFloat2 scale)
	{
		scale.x += scale.x;
		scale.y += scale.y;
		if (layerMoved)
			*layerMoved = true;
	}
}