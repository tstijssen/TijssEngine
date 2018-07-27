#pragma once

// base entity component definition

#include "TypeDefines.h"
#include "EntityTypes.h"

namespace Tijss
{
	class cEntity;

	class cEntityComponent
	{
	public:
		cEntityComponent(const string& type, TEntityUID UID, cEntity* entity)
		{
			m_Type = type;
			m_UID = UID;
			m_Entity = entity;
		}

		// Getters
		const string& GetType()
		{
			return m_Type;
		}
		TEntityUID GetUID()
		{
			return m_UID;
		}
		cEntity* GetEntity()
		{
			return m_Entity;
		}

		virtual bool Update(float32 updateTime) = 0;

	private:
		string		m_Type;
		TEntityUID	m_UID;
		cEntity*	m_Entity;
	};

}