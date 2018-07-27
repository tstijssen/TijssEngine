#include <algorithm>

#include "AnimatorComponent.h"
#include "EntityManager.h"
#include "RenderComponent.h"

namespace Tijss
{

	// Reference to entity messenger
	extern cMessenger Messenger;

	template <typename T>
	T clip(const T& n, const T& lower, const T& upper) {
		return (std::max)(lower, (std::min)(n, upper));
	}

	cAnimatorComponent::cAnimatorComponent
	(
		cEntity*	parentEntity,
		TEntityUID	UID,
		sInt2 rowsColumns,
		sInt2 newSize
	)
	: cEntityComponent("AnimatorComponent", UID, parentEntity)
	{
		frameTimer = 0;

		animationDelay = 0.1f;	// default animation speed
		frameRows = rowsColumns.x;
		frameColumns = rowsColumns.y;

		SelectAnimation({ 0,0 }, { frameColumns, frameRows });

		cRenderComponent* spriteRenderer = (cRenderComponent*)GetEntity()->GetComponent("Renderer");
		Transform* transform = &GetEntity()->m_Transform;


		transform->spriteTable.x = spriteRenderer->GetBitmap()->GetSize().width / transform->size.x;
		transform->spriteTable.y = spriteRenderer->GetBitmap()->GetSize().height / transform->size.y;

		transform->size.x = newSize.x;
		transform->size.y = newSize.y;
		transform->position.x = transform->size.x / 2;
		transform->position.y = transform->size.y / 2;
		transform->isAnimated = true;
		transform->animationFrame = frameIndex;
	}
	
	bool cAnimatorComponent::Update(float32 updateTime)
	{
		if (frameTimer > 0.0f && !paused)
		{
			frameTimer -= updateTime;
		}
		else if (!paused)
		{
			// advance column frame
			frameIndex.x = (1 + frameIndex.x);
			if (frameIndex.x >= endFrame.x)
			{
				frameIndex.x = startFrame.x;
				frameIndex.y = (1 + frameIndex.y);
				if (frameIndex.y >= endFrame.y)
				{
					frameIndex.y = startFrame.y;
				}
			}
			animationFinished = false;

			if (endFrame == sInt2(frameIndex.x + 1, frameIndex.y))
			{
				animationFinished = true;
			}

			frameTimer = animationDelay;	// reset timer

			// send new animation frame to transform
			GetEntity()->m_Transform.animationFrame = frameIndex;
		}
		return true;
	}

	void cAnimatorComponent::SetSpeed(float32 speed)
	{
		frameTimer = speed;
	}

	void cAnimatorComponent::SelectAnimation(sInt2 start, sInt2 end)
	{
		startFrame = { clip<int>(start.x, 0, frameColumns - 1), clip<int>(start.y, 0, frameRows - 1) };
		endFrame = { clip<int>(end.x, 0, frameColumns + 1), clip<int>(end.y, 0, frameRows + 1) };
		frameIndex = startFrame;
	}
}