
#include "TextComponent.h"
#include "EntityManager.h"

namespace Tijss
{
	// Reference to entity messenger
	extern cMessenger Messenger;

	cTextComponent::cTextComponent(
		cEntity*		parentEntity,
		TEntityUID		UID,
		Graphics*	gfx,
		string fontName,
		float fontSize,
		string text /*= ""*/
	)
		: cEntityComponent("Text", UID, parentEntity)
	{
		std::wstring wsFont(fontName.begin(), fontName.end());
		std::wstring wsText(text.begin(), text.end());

		m_TextRenderer = new cText(gfx, wsFont, fontSize, wsText);

		m_Active = true;
		//m_Position = { 0,0 };
		
		m_TextRenderer->SetPosition(GetEntity()->m_Transform.position);
	}

	bool cTextComponent::Update(float32 updateTime)
	{
		if (m_Active)
		{
			m_TextRenderer->Draw();
		}
		return true;
	}

	// change text position relative to entity position (0,0 for text is centre of entity pos)
	void cTextComponent::SetPosition(sFloat2 xy)
	{
		m_TextRenderer->SetPosition(xy);
		m_Position = GetEntity()->m_Transform.position + xy;
	}

	// change text position relative to entity position (0,0 for text is centre of entity pos)
	void cTextComponent::SetPosition(float32 x, float32 y)
	{
		sFloat2 tmp = { x,y };
		m_TextRenderer->SetPosition(tmp);

		m_Position.x = GetEntity()->m_Transform.position.x + x;
		m_Position.y = GetEntity()->m_Transform.position.x + y;
	}

	void cTextComponent::ChangeColour(sColour colour)
	{
		m_TextRenderer->SetColour(colour);
	}

	void cTextComponent::ChangeText(string newText)
	{
		// convert string to wstring
		std::wstring wsTmp(newText.begin(), newText.end());

		m_TextRenderer->SetText(wsTmp);
	}

}