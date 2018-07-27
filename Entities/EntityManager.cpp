#include "EntityManager.h"
#include <algorithm>
#include "RenderComponent.h"
#include "CollisionComponent.h"

namespace Tijss
{
	// Constructors/Destructors
	cEntityManager::cEntityManager(const string& filePath)
	{
		m_Entities.reserve(1024);

		m_EntityUIDMap = new Hash::cHashTable<TEntityUID, uint32>(2048, Hash::JenkSinglesHash);
		m_FolderPath = filePath;

		m_NextUID = 0;
		m_IsEnumerating = false;
	}

	cEntityManager::~cEntityManager()
	{
		DestroyAllEntities();
	}

	// Template creation/destruction

	cEntityTemplate* cEntityManager::CreateTemplate(const string& type, const string& name)
	{
		cEntityTemplate* newTemplate = new cEntityTemplate(type, name);

		m_Templates[name] = newTemplate;
		return newTemplate;
	}

	bool cEntityManager::DestroyTemplate(const string& name)
	{
		TemplateMapIter entityTemplate = m_Templates.find(name);

		if (entityTemplate == m_Templates.end())
		{
			return false;	// template to delete not found
		}

		delete entityTemplate->second;
		m_Templates.erase(entityTemplate);
		return true;
	}

	void cEntityManager::DestroyAllTemplates()
	{
		while (m_Templates.size())
		{
			TemplateMapIter entityTemplate = m_Templates.begin();
			while (entityTemplate != m_Templates.end())
			{
				delete entityTemplate->second;
				++entityTemplate;
			};
			m_Templates.clear();
		}
	}


	// entities creation/destruct
	TEntityUID cEntityManager::CreateEntity(const string& templateName, const string& name /*= ""*/)
	{
		cEntityTemplate* entityTemplate = GetTemplate(templateName);

		cEntity* newEntity = new cEntity(entityTemplate, m_NextUID, this, name);

		uint32 entityIndex = static_cast<uint32>(m_Entities.size());
		m_Entities.push_back(newEntity);

		m_EntityUIDMap->SetKeyValue(m_NextUID, entityIndex);
		
		m_IsEnumerating = false;
		return m_NextUID++;
	}

	// Destroy the given entity - returns true if the entity existed and was destroyed
	bool cEntityManager::DestroyEntity(TEntityUID UID)
	{
		// Find the vector index of the given UID
		uint32 entityIndex;
		if (!m_EntityUIDMap->LookUpKey(UID, &entityIndex))
		{
			// Quit if not found
			return false;
		}

		// Delete the given entity and remove from UID map
		delete m_Entities[entityIndex];
		m_EntityUIDMap->RemoveKey(UID);

		// If not removing last entity...
		if (entityIndex != m_Entities.size() - 1)
		{
			// ...put the last entity into the empty entity slot and update UID map
			m_Entities[entityIndex] = m_Entities.back();
			m_EntityUIDMap->SetKeyValue(m_Entities.back()->GetUID(), entityIndex);
		}
		m_Entities.pop_back(); // Remove last entity

		m_IsEnumerating = false;
		return true;
	}


	// Destroy all entities held by the manager
	void cEntityManager::DestroyAllEntities()
	{
		m_EntityUIDMap->RemoveAllEntries();
		while (m_Entities.size())
		{
			delete m_Entities.back();
			m_Entities.pop_back();
		}

		m_IsEnumerating = false;
	}

	void cEntityManager::UpdateAllEntities(float updateTime)
	{
		uint32 entity = 0;
		while (entity < m_Entities.size())
		{
			// Update entity, if it returns false, then destroy it
			if (!m_Entities[entity]->Update(updateTime))
			{
				DestroyEntity(m_Entities[entity]->GetUID());
			}
			else
			{
				++entity;
			}
		}
	}

	void cEntityManager::AddDrawableEntity(cRenderComponent* renderer, int32 layer, bool isometric)
	{
		if (layer >= SceneLayers.size())
		{
			for (int i = SceneLayers.size(); i <= layer; ++i)
			{
				spriteLayer newLayer;
				SceneLayers.push_back(newLayer);
			}
		}

		// layers are non-isometric by default
		if (isometric && !SceneLayers.at(layer).isIsomentric)
		{
			SceneLayers.at(layer).isIsomentric = true;
		}
		SceneLayers.at(layer).m_Renderers.push_back(renderer);
		renderer->GetEntity()->m_Transform.layerMoved = &SceneLayers.at(layer).hasMoved;
	}

	void cEntityManager::AddCollidingEntity(cColliderComponent* collider, int32 layer)
	{
		if (layer >= SceneLayers.size())
		{
			for (int i = SceneLayers.size(); i <= layer; ++i)
			{
				spriteLayer newLayer;
				SceneLayers.push_back(newLayer);
			}
		}
			
		SceneLayers.at(layer).m_CollidingEntities.push_back(collider);
	}

	// function for sorting isometric layers
	struct isometric_sort
	{
		inline bool operator() (cRenderComponent* spriteA, cRenderComponent* spriteB)
		{
			return (spriteA->GetEntity()->m_Transform.position.y + (spriteA->GetEntity()->m_Transform.size.y / 2) < 
					spriteB->GetEntity()->m_Transform.position.y + (spriteB->GetEntity()->m_Transform.size.y / 2));
		}
	};

	void cEntityManager::DrawAllEntities(Graphics* gfx)
	{
		vector<spriteLayer>::iterator layerIter = SceneLayers.end();
		// render layers backwards. 0 is closest to the camera (used as UI layer)
		while (layerIter != SceneLayers.begin())
		{
			--layerIter;

			if ((*layerIter).active)
			{
				if ((*layerIter).isIsomentric && (*layerIter).hasMoved)
				{
					// if something in an isometric layer has moved, sort the layer
					std::sort((*layerIter).m_Renderers.begin(), (*layerIter).m_Renderers.end(), isometric_sort());
				}
				gfx->GetContext()->PushLayer(
					D2D1::LayerParameters(D2D1::InfiniteRect(),
						NULL,
						D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
						D2D1::IdentityMatrix(),
						1.0f,
						(ID2D1Brush*)0,
						D2D1_LAYER_OPTIONS_NONE
					),
					(*layerIter).renderLayer
				);
				for each(cRenderComponent* drawable in (*layerIter).m_Renderers)
				{
					if (drawable->m_Active)
					{
						drawable->Render(gfx);
					}
				}
				gfx->GetContext()->PopLayer();
				(*layerIter).hasMoved = false;
			}	
		}
	}

	void cEntityManager::DetectCollisions(TEntityUID targetUID)
	{
		// grab collider component
		cColliderComponent* targetCollider = (cColliderComponent*)GetEntity(targetUID)->GetComponent("Collider");
		targetCollider->ClearCollisions();

		// get layer of target
		uint32 targetLayer = GetEntity(targetUID)->m_Transform.layer;

		int32 numCollidingEntities = SceneLayers.at(targetLayer).m_CollidingEntities.size();

		if (numCollidingEntities < (threader.m_NumWorkers + 1))
		{
			for (uint32_t t = 0; t < numCollidingEntities; ++t)
			{
				auto& work = threader.m_CollisionWorkers[t].second;

				work.target = targetCollider;

				work.start = SceneLayers.at(targetLayer).m_CollidingEntities.begin() + t;
				work.end = SceneLayers.at(targetLayer).m_CollidingEntities.begin() + (t + 1);

				auto& workerThread = threader.m_CollisionWorkers[t].first;
				{
					std::unique_lock<std::mutex> lock(workerThread.lock);
					work.complete = false;
				}

				workerThread.workReady.notify_one();
			}
		}
		else
		{
			float roundedSpritesNum = ceil(1.0f * SceneLayers.at(targetLayer).m_CollidingEntities.size() / (threader.m_NumWorkers + 1));
			uint32_t SpritesPerThread = roundedSpritesNum;
			uint32_t threadSelection = 0;

			for (uint32_t t = 0; t < threader.m_NumWorkers; ++t)
			{
				auto& work = threader.m_CollisionWorkers[t].second;

				work.target = targetCollider;

				work.start = SceneLayers.at(targetLayer).m_CollidingEntities.begin() + threadSelection;
				work.end = SceneLayers.at(targetLayer).m_CollidingEntities.begin() + (threadSelection + SpritesPerThread);

				auto& workerThread = threader.m_CollisionWorkers[t].first;
				{
					std::unique_lock<std::mutex> lock(workerThread.lock);
					work.complete = false;
				}

				workerThread.workReady.notify_one();

				threadSelection += SpritesPerThread;
				// exit loop
				if (threadSelection + SpritesPerThread > SpritesPerThread)
				{
					t = threader.m_NumWorkers + 1;
				}
			}

			threader.ComponentCollisions(targetCollider, SceneLayers.at(targetLayer).m_CollidingEntities.begin() + threadSelection, SceneLayers.at(targetLayer).m_CollidingEntities.end());

		}

		for (int t = 0; t < threader.m_NumWorkers; ++t)
		{
			auto& work = threader.m_CollisionWorkers[t].second;
			auto& workerThread = threader.m_CollisionWorkers[t].first;

			std::unique_lock<std::mutex> lock(workerThread.lock);
			workerThread.workReady.wait(lock, [&]() {return work.complete; });

		}
	}
}