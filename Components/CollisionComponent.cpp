#include "EntityManager.h"
#include "CollisionComponent.h"

namespace Tijss
{

	// Reference to entity messenger
	extern cMessenger Messenger;

	cColliderComponent::cColliderComponent
	(
		cEntity*		parentEntity,
		TEntityUID		UID,
		ColliderType	collider,
		uint32			layer,
		float32			radius /*= 0*/,
		sFloat2			boxSize /*= {0,0}*/
	): cEntityComponent("Collider", UID, parentEntity)
	{
		m_Type = collider;
		if (radius == 0)
		{
			m_Radius = parentEntity->m_Transform.size.x / 2;
		}
		else
		{
			m_Radius = radius;
		}

		if (boxSize.x == 0 && boxSize.y == 0)
		{
			m_BoxSize = parentEntity->m_Transform.size;
		}
		else
		{
			m_BoxSize = boxSize;
		}
		GetEntity()->GetManager()->AddCollidingEntity(this, layer);
	}

	bool cColliderComponent::CheckCollisions(cColliderComponent* other)
	{
		Tijss::ColliderType targetCollider = other->GetColliderType();
		sFloat2 targetPosition = other->GetEntity()->m_Transform.position;
		sFloat2 sourcePosition = this->GetEntity()->m_Transform.position;
		bool hasCollided = false;

		switch (targetCollider)
		{
		case ColliderType::Box:
		{
			sFloat2 targetSize = other->GetBoxSize();
			sFloat2 targetMinXY = targetPosition;
			targetMinXY.x -= targetSize.x / 2;
			targetMinXY.y -= targetSize.y / 2;

			sFloat2 targetMaxXY = targetPosition;
			targetMaxXY.x += targetSize.x / 2;
			targetMaxXY.y += targetSize.y / 2;

			switch (m_Type)
			{
				case ColliderType::Box:
				{
					sFloat2 sourceSize = this->GetBoxSize();
					sFloat2 sourceMinXY = sourcePosition;
					sourceMinXY.x -= sourceSize.x / 2;
					sourceMinXY.y -= sourceSize.y / 2;
					sFloat2 sourceMaxXY = sourcePosition;
					sourceMaxXY.x += sourceSize.x / 2;
					sourceMaxXY.y += sourceSize.y / 2;

					if ((abs(targetMinXY.x - sourceMinXY.x) * 2 < (targetMaxXY.x + sourceMaxXY.x)) &&
						(abs(targetMinXY.y - sourceMinXY.y) * 2 < (targetMaxXY.y + sourceMaxXY.y)))
					{
						hasCollided = true;
					}
					break;
				}
				case ColliderType::Circle:
				{
					auto sourceRadius = this->GetRadius();

					auto halfWidth = targetSize.x / 2;
					auto halfHeight = targetSize.y / 2;

					auto distX = abs(sourcePosition.x - targetMinXY.x - halfWidth);
					auto distY = abs(sourcePosition.y - targetMinXY.y - halfHeight);

					auto pX = distX - halfWidth;
					auto pY = distY - halfHeight;

					if (distX > (halfWidth + sourceRadius))
					{
						hasCollided = false;
					}
					else if (distY > (halfHeight + sourceRadius))
					{
						hasCollided = false;
					}
					else if (distX <= (halfWidth))
					{
						hasCollided = true;
					}
					else if (distY <= (halfHeight))
					{
						hasCollided = true;
					}
					else if (pX * pX + pY * pY <= (sourceRadius * sourceRadius))
					{
						hasCollided = true;
					}
					break;
				}
				case ColliderType::Point:
				{
					if (sourcePosition.x > targetMinXY.x && sourcePosition.x < targetSize.x && 
						sourcePosition.y > targetMinXY.y && sourcePosition.y < targetSize.y)
					{
						hasCollided = true;
					}
					break;
				}
			}
			break;
		}
		case ColliderType::Circle:
		{
			auto targetRadius = other->GetRadius();

			switch (m_Type)
			{
			case ColliderType::Box:
			{
				sFloat2 sourceSize = this->GetBoxSize();
				sFloat2 sourceMinXY = sourcePosition;
				sourceMinXY.x -= sourceSize.x / 2;
				sourceMinXY.y -= sourceSize.y / 2;

				auto halfWidth = sourceSize.x / 2;
				auto halfHeight = sourceSize.y / 2;

				auto distX = abs(sourceMinXY.x - targetPosition.x - halfWidth);
				auto distY = abs(sourceMinXY.y - targetPosition.y - halfHeight);

				auto pX = distX - halfWidth;
				auto pY = distY - halfHeight;

				if (distX > (halfWidth + targetRadius))
				{
					hasCollided = false;
				}
				else if (distY > (halfHeight + targetRadius))
				{
					hasCollided = false;
				}
				else if (distX <= (halfWidth))
				{
					hasCollided = true;
				}
				else if (distY <= (halfHeight))
				{
					hasCollided = true;
				}
				else if (pX * pX + pY * pY <= (targetRadius * targetRadius))
				{
					hasCollided = true;
				}
				break;
			}
			case ColliderType::Circle:
			{
				auto x = abs(sourcePosition.x + targetPosition.x);
				auto y = abs(sourcePosition.y + targetPosition.y);
				auto distance = sqrt(x * x + y * y);
				auto sourceRadius = this->GetRadius();

				if (distance < targetRadius + sourceRadius)
				{
					hasCollided = true;
				}
				break;
			}
			case ColliderType::Point:
			{
				auto x = abs(targetPosition.x + sourcePosition.x);
				auto y = abs(targetPosition.y + sourcePosition.y);
				auto distance = sqrt(x * x + y * y);

				if (distance < targetRadius)
				{
					hasCollided = true;
				}
				break;
			}
			}
			break;
		}
		case ColliderType::Point:
		{
			switch (m_Type)
			{
			case ColliderType::Box:
			{
				sFloat2 sourceSize = this->GetBoxSize();
				sFloat2 sourceMinXY = sourcePosition;
				sourceMinXY.x -= sourceSize.x / 2;
				sourceMinXY.y -= sourceSize.y / 2;
				sFloat2 sourceMaxXY = sourcePosition;
				sourceMaxXY.x += sourceSize.x / 2;
				sourceMaxXY.y += sourceSize.y / 2;

				if (targetPosition.x > sourceMinXY.x && targetPosition.x < sourceMaxXY.x &&
					targetPosition.y > sourceMinXY.y && targetPosition.y < sourceMaxXY.y)
				{
					hasCollided = true;
				}
				break;
			}
			case ColliderType::Circle:
			{
				auto x = abs(sourcePosition.x + targetPosition.x);
				auto y = abs(sourcePosition.y + targetPosition.y);
				auto distance = sqrt(x * x + y * y);
				auto sourceRadius = this->GetRadius();

				if (distance < sourceRadius)
				{
					hasCollided = true;
				}
				break;
			}
			case ColliderType::Point:
			{
				if (sourcePosition == targetPosition)
				{
					hasCollided = true;
				}
				break;
			}
			}
			break;
		}
		}

		if (hasCollided)
		{
			other->m_Collisions.push_back(this->GetEntity()->GetUID());
			return true;
		}
		return false;
	}

	bool cColliderComponent::DetectPointCollision(sFloat2 xy)
	{
		sFloat2 sourcePosition = this->GetEntity()->m_Transform.position;

		switch (m_Type)
		{
		case ColliderType::Box:
		{
			sFloat2 sourceSize = GetEntity()->m_Transform.size;
			sFloat2 sourceMinXY = sourcePosition;
			sourceMinXY.x -= sourceSize.x / 2;
			sourceMinXY.y -= sourceSize.y / 2;
			sFloat2 sourceMaxXY = sourcePosition;
			sourceMaxXY.x += sourceSize.x / 2;
			sourceMaxXY.y += sourceSize.y / 2;

			if (xy.x > sourceMinXY.x && xy.x < sourceMaxXY.x &&
				xy.y > sourceMinXY.y && xy.y < sourceMaxXY.y)
			{
				return true;
			}
			break;
		}
		case ColliderType::Circle:
		{
			auto x = abs(sourcePosition.x + xy.x);
			auto y = abs(sourcePosition.y + xy.y);
			auto distance = sqrt(x * x + y * y);
			auto sourceRadius = this->GetRadius();

			if (distance < sourceRadius)
			{
				return true;

			}
			break;
		}
		case ColliderType::Point:
		{
			if (sourcePosition == xy)
			{
				return true;
			}
			break;
		}
		}
		return false;
	}
}