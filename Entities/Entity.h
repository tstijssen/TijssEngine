#pragma once

#include <map>
#include <string>

using namespace std;
#include "EntityTypes.h"
#include "EntityComponent.h"

namespace Tijss
{
	class cEntityManager;

	struct Transform
	{
		sFloat2 prevPosition;
		sFloat2 position;
		sFloat2 size;
		sFloat2 origin;
		sFloat2 scale;
		sInt2 animationFrame;
		sInt2 spriteTable;
		uint32 layer;
		bool isAnimated;
		float rotation;
		bool *layerMoved;

		void Scale(float x, float y);
		void Scale(float s);
		void Scale(sFloat2 scale);

		void Rotate(float angle);
		void SetRotation(float rotation);

		void Move(int x, int y);
		void Move(sFloat2 vector);

		void SetPosition(sFloat2 position);
		void SetPosition(int x, int y);
	};

	// common data for all entities
	class cEntityTemplate
	{
	public:
		cEntityTemplate(const string& type, const string& name)
		{
			m_Type = type;
			m_Name = name;
		}
		//	Getters

		const string& GetType()
		{
			return m_Type;
		}

		const string& GetName()
		{
			return m_Name;
		}
	private:
		// prevent use of copy and assignment
		cEntityTemplate(const cEntityTemplate&);
		cEntityTemplate& operator=(const cEntityTemplate&);

	private:
		string m_Type;
		string m_Name;
	};

	// basic entity class

	class cEntity
	{
	public:
		cEntity
		(
			cEntityTemplate* entityTemplate,
			TEntityUID UID,
			cEntityManager* entityManager,
			const string& name = ""
		);

		Transform m_Transform;
		bool m_Active;
		TEntityUID GetUID()
		{
			return m_UID;
		}

		cEntityTemplate* const GetTemplate()
		{
			return m_Template;
		}

		cEntityManager* const GetManager()
		{
			return m_Manager;
		}

		const string& GetName()
		{
			return m_Name;
		}
		
		void AddComponent(cEntityComponent* component);

		cEntityComponent* GetComponent(TEntityUID UID);

		cEntityComponent* GetComponent(const string& type);

		bool RemoveComponent(TEntityUID UID);
		bool RemoveComponent(const string& type);

		void RemoveAllComponents();

		bool Update(float32 updateTime);

		// make render a component

	private:
		cEntity(const cEntity&);
		cEntity& operator=(const cEntity&);

		// data
		cEntityTemplate* m_Template;
		cEntityManager* m_Manager;

		TEntityUID m_UID;
		string m_Name;

		map<TEntityUID, cEntityComponent*> m_Components;

	};

}