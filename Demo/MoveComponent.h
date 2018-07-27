#pragma once
#include "TypeDefines.h"
#include "EntityTypes.h"
#include "EntityComponent.h"
#include "Messenger.h"
#include "Graphics.h"

namespace Tijss
{
	class cEntity;
	class cAnimatorComponent;

	enum eCharacterDirection { North, South, West, East, SouthWest, SouthEast, NorthWest, NorthEast };

	class cMoveComponent : public cEntityComponent
	{
	public:
		cMoveComponent(
			cEntity*		parentEntity,
			TEntityUID		UID,
			float32			moveSpeed,
			Camera*			camera
		);

		bool m_Active;
		bool Update(float32 updateTime);
	private:
		float32 m_Speed;
		cAnimatorComponent* m_Animation;
		eCharacterDirection m_Direction;
		Camera*	m_Camera;
		bool m_Moving;
	};

}