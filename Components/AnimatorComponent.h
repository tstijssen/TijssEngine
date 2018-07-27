#pragma once

#include "TypeDefines.h"
#include "EntityTypes.h"
#include "EntityComponent.h"
#include "Messenger.h"

namespace Tijss
{
	class cEntity;

	class cAnimatorComponent: public cEntityComponent
	{
	public:
		cAnimatorComponent
		(
			cEntity*	parentEntity,
			TEntityUID	UID,
			sInt2 rowsColumns,
			sInt2 newSize
		);

		bool Update(float32 updateTime);

		bool animationFinished;	// whether this frame the animation loops back
		bool paused;

		void SelectAnimation(sInt2 start, sInt2 end);
		void SetSpeed(float32 speed);
	private:
		int frameColumns;	// number of columns in the spritesheet
		int frameRows;		// number of rows in the spritesheet
		sInt2 frameIndex;
		sInt2 startFrame;
		sInt2 endFrame;

		float frameTimer;	// countdown to next frame
		float animationDelay; // time in seconds for next frame
	};
}