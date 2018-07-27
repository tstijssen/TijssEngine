#include "MoveComponent.h"
#include "EntityManager.h"
#include "CollisionComponent.h"
#include "Input.h"
#include "AnimatorComponent.h"

namespace Tijss
{
	// Reference to entity messenger
	extern cMessenger Messenger;

	cMoveComponent::cMoveComponent(
		cEntity*		parentEntity,
		TEntityUID		UID,
		float32			moveSpeed,
		Camera*			camera
	)
		: cEntityComponent("PlayerMovement", UID, parentEntity)
	{
		m_Speed = moveSpeed;
		m_Active = true;
		m_Camera = camera;

		m_Moving = false;
		m_Direction = SouthEast;
		m_Animation = (cAnimatorComponent*)GetEntity()->GetComponent("AnimatorComponent");
	}

	bool cMoveComponent::Update(float32 updateTime)
	{
		if (m_Active)
		{
			Transform* m_EntityTransform = &GetEntity()->m_Transform;

			sFloat2 prevPos = m_EntityTransform->position;
			eCharacterDirection prevDirection = m_Direction;
			bool movingLastFrame = m_Moving;

			// move 'up'
			if (KeyHeld(Key_W))
			{
				m_EntityTransform->Move({ 0, -1 * m_Speed * updateTime });
				m_Camera->Move(0, 1 * m_Speed * updateTime);
				m_Direction = North;
				m_Moving = true;

				// move 'left'
				if (KeyHeld(Key_A))
				{
					m_EntityTransform->Move({ -1 * m_Speed * updateTime, 0 });
					m_Camera->Move(1 * m_Speed * updateTime, 0);
					m_Direction = NorthWest;
				}

				// move 'right'
				if (KeyHeld(Key_D))
				{
					m_EntityTransform->Move({ 1 * m_Speed * updateTime, 0 });
					m_Camera->Move(-1 * m_Speed * updateTime, 0);
					m_Direction = NorthEast;
				}
			}
			// move 'down'
			else if (KeyHeld(Key_S))
			{
				m_EntityTransform->Move({ 0, 1 * m_Speed * updateTime });
				m_Camera->Move(0, -1 * m_Speed * updateTime);
				m_Direction = South;
				m_Moving = true;

				// move 'left'
				if (KeyHeld(Key_A))
				{
					m_EntityTransform->Move({ -1 * m_Speed * updateTime, 0 });
					m_Camera->Move(1 * m_Speed * updateTime, 0);
					m_Direction = SouthWest;
				}

				// move 'right'
				if (KeyHeld(Key_D))
				{
					m_EntityTransform->Move({ 1 * m_Speed * updateTime, 0 });
					m_Camera->Move(-1 * m_Speed * updateTime, 0);
					m_Direction = SouthEast;
				}
			}
			// move 'left'
			else if (KeyHeld(Key_A))
			{
				m_EntityTransform->Move({ -1 * m_Speed * updateTime, 0 });
				m_Camera->Move(1 * m_Speed * updateTime, 0);
				m_Moving = true;
				m_Direction = West;
			}
			// move 'right'
			else if (KeyHeld(Key_D))
			{
				m_EntityTransform->Move({ 1 * m_Speed * updateTime, 0 });
				m_Camera->Move(-1 * m_Speed * updateTime, 0);
				m_Moving = true;
				m_Direction = East;
			}
			else
				m_Moving = false;

			// set animations if direction changed or stopped/started moving
			if (m_Direction != prevDirection || m_Moving != movingLastFrame)
			{
				switch (m_Direction)
				{
				case North:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,2 }, { 11,2 });
					else
						m_Animation->SelectAnimation({ 0,2 }, { 3,2 });
					break;

				case South:

					if (m_Moving)
						m_Animation->SelectAnimation({ 5,6 }, { 11,6 });
					else
						m_Animation->SelectAnimation({ 0,6 }, { 3,6 });
					break;

				case West:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,0 }, { 11,0 });
					else
						m_Animation->SelectAnimation({ 0,0 }, { 3,0 });
					break;

				case East:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,4 }, { 11,4 });
					else
						m_Animation->SelectAnimation({ 0,4 }, { 3,4 });
					break;

				case NorthWest:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,1 }, { 11,1 });
					else
						m_Animation->SelectAnimation({ 0,1 }, { 3,1 });
					break;

				case NorthEast:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,3 }, { 11,3 });
					else
						m_Animation->SelectAnimation({ 0,3 }, { 3,3 });
					break;

				case SouthWest:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,7 }, { 11,7 });
					else
						m_Animation->SelectAnimation({ 0,7 }, { 3,7 });
					break;

				case SouthEast:
					if (m_Moving)
						m_Animation->SelectAnimation({ 5,5 }, { 11,5 });
					else
						m_Animation->SelectAnimation({ 0,5 }, { 3,5 });
					break;
			}
		}

			GetEntity()->GetManager()->DetectCollisions(GetEntity()->GetUID());
			
			cColliderComponent* collider = (cColliderComponent*)GetEntity()->GetComponent("Collider");

			// if any collisions have occured
			if (collider->GetCollisions().size() > 0)
			{
				m_EntityTransform->position = prevPos;
			}

			//for each(TEntityUID other in collider->GetCollisions())
			//{
			//
			//}

		}
		return true;
	}


}