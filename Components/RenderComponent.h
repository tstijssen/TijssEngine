#pragma once

#include "TypeDefines.h"
#include "EntityTypes.h"
#include "EntityComponent.h"
#include "Messenger.h"
#include <d2d1_3.h>

namespace Tijss
{
	class cEntity;
	class Graphics;

	class cRenderComponent : public cEntityComponent
	{
	public:
		cRenderComponent
		(
			cEntity*	parentEntity,
			TEntityUID	UID,
			string		spriteName,
			Graphics*	gfx,
			uint32		layer,
			bool		isometric = false,
			bool		UI = false
		);

		bool m_Active;
		bool Update(float32 updateTime);
		bool Render(Graphics* gfx);

		ID2D1Bitmap1 * GetBitmap()
		{
			return bmp;
		}

	private:
		ID2D1Bitmap1 * bmp;

		int m_Layer;
		bool m_Interface;
		bool m_Isometric;

		// data for animated sprites, unused otherwise
		//sInt2 m_SpritesTable;	// number of individual sprites in the bitmap
	};
}