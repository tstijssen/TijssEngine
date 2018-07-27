#pragma once

#include "TypeDefines.h"
#include "EntityTypes.h"
#include "EntityComponent.h"
#include "Messenger.h"
#include "TextRender.h"


namespace Tijss
{
	class cEntity;

	class cTextComponent : public cEntityComponent
	{
	public:
		cTextComponent(
			cEntity*		parentEntity,
			TEntityUID		UID,
			Graphics*	gfx, 
			string fontName,
			float fontSize, 
			string text = "");

		bool m_Active;
		bool Update(float32 updateTime);

		// change text position relative to entity position (0,0 for text is centre of entity pos)
		void SetPosition(sFloat2 xy);
		// change text position relative to entity position (0,0 for text is centre of entity pos)
		void SetPosition(float32 x, float32 y);

		void ChangeColour(sColour colour);
		void ChangeText(string newText);

		~cTextComponent()
		{
			delete m_TextRenderer;
		}

	private:
		sFloat2 m_Position;
		cText* m_TextRenderer;
	};
}