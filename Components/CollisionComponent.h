#pragma once

#include "TypeDefines.h"
#include "EntityTypes.h"
#include "EntityComponent.h"
#include "Messenger.h"

namespace Tijss
{
	class cEntity;

	enum ColliderType { Circle, Box, Point };

	class cColliderComponent : public cEntityComponent
	{
	public:
		cColliderComponent
		(
			cEntity*		parentEntity,
			TEntityUID		UID,
			ColliderType	collider,
			uint32			layer,
			float32			radius = 0,
			sFloat2			boxSize = {0,0}
		);

		bool m_Active;
		bool Update(float32 updateTime) { return true; };
		bool CheckCollisions(cColliderComponent* other);
		bool DetectPointCollision(sFloat2 xy);

		ColliderType GetColliderType()
		{
			return m_Type;
		}

		std::vector<TEntityUID> GetCollisions()
		{
			return m_Collisions;
		}

		// clear vector holding detected collisions
		void ClearCollisions()
		{
			m_Collisions.clear();
		}

		float32 GetRadius()
		{
			return m_Radius;
		}

		sFloat2 GetBoxSize()
		{
			return m_BoxSize;
		}

	private:
		ColliderType m_Type;
		sFloat2 m_BoxSize;
		float32 m_Radius;
		std::vector<TEntityUID> m_Collisions;
	};
}