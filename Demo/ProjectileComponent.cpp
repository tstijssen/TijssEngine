#include "ProjectileComponent.h"
#include "EntityManager.h"
#include "CollisionComponent.h"
#include "Input.h"

namespace Tijss
{
	// Reference to entity messenger
	extern cMessenger Messenger;

	cProjectileComponent::cProjectileComponent(
		cEntity*		parentEntity,
		TEntityUID		UID,
		float32			moveSpeed,
		bool			moveUp
	)
		: cEntityComponent("ProjectileMovement", UID, parentEntity)
	{
		m_Speed = moveSpeed;
		m_Active = true;
		m_ExplosionSound = new Audio::cSound("explosion1.wav");
		m_ExplosionSound->m_Sound.setLoop(false);
		m_MovingUp = moveUp;
	}

	bool cProjectileComponent::Update(float32 updateTime)
	{
		if (GetEntity()->m_Active)
		{
			if (m_MovingUp)
				GetEntity()->m_Transform.Move(0, -1 * m_Speed * updateTime);
			else
				GetEntity()->m_Transform.Move(0, 1 * m_Speed * updateTime);

			GetEntity()->GetManager()->DetectCollisions(GetEntity()->GetUID());

			cColliderComponent* collider = (cColliderComponent*)GetEntity()->GetComponent("Collider");

			if (collider->GetCollisions().size() > 0)
			{
				GetEntity()->m_Active = false;
				GetEntity()->GetManager()->DestroyEntity(collider->GetCollisions().at(0));
				collider->ClearCollisions();
			}

			if (m_MovingUp)
			{
				if (GetEntity()->m_Transform.position.y < 0)
				{
					GetEntity()->m_Active = false;
				}
			}
			else
			{
				if (GetEntity()->m_Transform.position.y > 800)
				{
					GetEntity()->m_Active = false;
				}
			}
		}

		return true;
	}

}