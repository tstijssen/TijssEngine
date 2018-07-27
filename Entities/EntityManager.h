#pragma once

#include <map>
#include <vector>

using namespace std;

#include "TypeDefines.h"
#include "Entity.h"
#include "Graphics.h"
#include "Multithreading.h"
#include "HashTable.h"

namespace Tijss
{
	class cRenderComponent;
	class cColliderComponent;

	class cEntityManager
	{
	public:
		cEntityManager(const string& filePath);
		~cEntityManager();

	private:
		cEntityManager(const cEntityManager&);	//block copy constructor or assignment
		cEntityManager& operator=(const cEntityManager&);

	public:

		// template construction/destruction
		cEntityTemplate * cEntityManager::CreateTemplate(const string& type, const string& name);

		bool DestroyTemplate(const string& name);

		void DestroyAllTemplates();

		// entity construction/destruction

		TEntityUID CreateEntity(const string& templateName, const string& entityName);

		bool DestroyEntity(TEntityUID UID);

		void DestroyAllEntities();

		TEntityUID GetNewUID()
		{
			return m_NextUID++;
		}

		cEntityTemplate* GetTemplate(const string& name)
		{
			map<string, cEntityTemplate*>::iterator entityTemplate = m_Templates.find(name);

			if (entityTemplate == m_Templates.end())
			{
				return 0;	// template name not found in list
			}

			return (*entityTemplate).second;
		}

		uint32 NumOfEntities()
		{
			return static_cast<uint32>(m_Entities.size());
		}

		// return entity at index
		cEntity* GetEntityAt(uint32 index)
		{
			return m_Entities[index];
		}

		cEntity* GetEntity(TEntityUID UID)
		{
			uint32 entityIndex;
			if (!m_EntityUIDMap->LookUpKey(UID, &entityIndex))
			{
				return 0;
			}
			return m_Entities[entityIndex];
		}

		// Return the entity with the given name & optionally the given template name & type
		cEntity* GetEntity(const string& name, const string& templateName = "",
			const string& templateType = "")
		{
			EntityIterator entity = m_Entities.begin();
			while (entity != m_Entities.end())
			{
				if ((*entity)->GetName() == name &&
					(templateName.length() == 0 || (*entity)->GetTemplate()->GetName() == templateName) &&
					(templateType.length() == 0 || (*entity)->GetTemplate()->GetType() == templateType))
				{
					return (*entity);
				}
				++entity;
			}
			return 0;
		}

		void BeginEnumEntities(const string& name, const string& templateName,
			const string& templateType = "")
		{
			m_IsEnumerating = true;
			m_EnumEntity = m_Entities.begin();
			m_EnumName = name;
			m_EnumTemplateName = templateName;
			m_EnumTemplateType = templateType;
		}

		void EndEnumEntities()
		{
			m_IsEnumerating = false;
		}

		cEntity* EnumEntity()
		{
			if (!m_IsEnumerating)
			{
				return 0;
			}

			while (m_EnumEntity != m_Entities.end())
			{
				if ((m_EnumName.length() == 0 || (*m_EnumEntity)->GetName() == m_EnumName) &&
					(m_EnumTemplateName.length() == 0 ||
					(*m_EnumEntity)->GetTemplate()->GetName() == m_EnumTemplateName) &&
						(m_EnumTemplateType.length() == 0 ||
					(*m_EnumEntity)->GetTemplate()->GetType() == m_EnumTemplateType))
				{
					cEntity* foundEntity = *m_EnumEntity;
					++m_EnumEntity;
					return foundEntity;
				}
				++m_EnumEntity;
			}

			m_IsEnumerating = false;
			return 0;
		}

		void AddDrawableEntity(cRenderComponent* renderer, int32 layer, bool isometric);
		void AddCollidingEntity(cColliderComponent* collider, int32 layer);

		void UpdateAllEntities(float updateTime);
		void DetectCollisions(TEntityUID targetUID);
		void DrawAllEntities(Graphics* gfx);

		// return path to folder used for loading sprites
		string GetFilepath()
		{
			return m_FolderPath;
		}

		// data
	private:

		typedef map<string, cEntityTemplate*> TemplateMap;
		typedef TemplateMap::iterator TemplateMapIter;

		typedef vector<cEntity*> EntityVector;
		typedef EntityVector::iterator EntityIterator;

		TemplateMap m_Templates;
		EntityVector m_Entities;

		struct spriteLayer
		{
			vector <cRenderComponent*> m_Renderers;
			vector <cColliderComponent*> m_CollidingEntities;
			ID2D1Layer * renderLayer = NULL;
			bool active;
			bool hasMoved;
			bool uiLayer = false;		// layers are not ui layers by default
			bool isIsomentric = false;	// layers are non-isometric by default
		};
		multiThread::MultiThreader threader;
		vector<spriteLayer> SceneLayers;
		string m_FolderPath;
		
		Hash::cHashTable<TEntityUID, uint32>* m_EntityUIDMap;

		TEntityUID m_NextUID;
		//Graphics* gfx;	// reference to graphics device, used for rendering

		// Data for Entity Enumeration
		bool        m_IsEnumerating;
		EntityIterator m_EnumEntity;
		string      m_EnumName;
		string      m_EnumTemplateName;
		string      m_EnumTemplateType;
	};
}
