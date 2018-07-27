#pragma once
#include "TypeDefines.h"
#include "EntityTypes.h"
#include "EntityComponent.h"
#include "Messenger.h"
#include "Sound.h"

namespace Tijss
{
	class cEntity;

	class cProjectileComponent : public cEntityComponent
	{
	public:
		cProjectileComponent(
			cEntity*		parentEntity,
			TEntityUID		UID,
			float32			moveSpeed,
			bool			moveUp
		);

		bool m_Active;
		bool Update(float32 updateTime);
		~cProjectileComponent()
		{
			delete m_ExplosionSound;
		}
	private:
		float32 m_Speed;
		bool m_MovingUp;
		Tijss::Audio::cSound* m_ExplosionSound;
	};

}