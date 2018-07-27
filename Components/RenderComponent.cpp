#include "RenderComponent.h"
#include "EntityManager.h"
#include <wincodec.h>

namespace Tijss
{
	// Reference to entity messenger
	extern cMessenger Messenger;

	cRenderComponent::cRenderComponent
	(
		cEntity*	parentEntity,
		TEntityUID	UID,
		string		spriteName,
		Graphics*	gfx,
		uint32		layer,
		bool		isometric, /*= false*/
		bool		UI /*= false*/
	)
		: cEntityComponent("Renderer", UID, parentEntity)
	{
		m_Layer = layer;
		m_Isometric = isometric;
		m_Interface = UI;
		// create sprite
		bmp = NULL;

		HRESULT hr;

		string path = GetEntity()->GetManager()->GetFilepath();
		path += spriteName;
		std::wstring widestr = std::wstring(path.begin(), path.end());

		// create path for sprite file
		const wchar_t* finalPath = widestr.c_str();

		// create factory
		IWICImagingFactory *wicFactory = NULL;

		hr = CoCreateInstance(
			CLSID_WICImagingFactory,
			NULL,
			CLSCTX_INPROC_SERVER,
			IID_IWICImagingFactory,
			(LPVOID*)&wicFactory
		);


		IWICBitmapDecoder *wicDecoder = NULL;
		hr = wicFactory->CreateDecoderFromFilename(
			finalPath,
			NULL,
			GENERIC_READ,
			WICDecodeMetadataCacheOnLoad,
			&wicDecoder
		);

		IWICBitmapFrameDecode* wicFrame = NULL;
		hr = wicDecoder->GetFrame(0, &wicFrame);

		IWICFormatConverter *wicConverter = NULL;
		hr = wicFactory->CreateFormatConverter(&wicConverter);

		hr = wicConverter->Initialize(
			wicFrame,
			GUID_WICPixelFormat32bppPBGRA,
			WICBitmapDitherTypeNone,
			NULL,
			0.0,
			WICBitmapPaletteTypeCustom
		);

		hr = gfx->GetContext()->CreateBitmapFromWicBitmap(
			wicConverter,
			NULL,
			&bmp
		);

		if (wicFactory) wicFactory->Release();
		if (wicDecoder) wicDecoder->Release();
		if (wicConverter) wicConverter->Release();
		if (wicFrame) wicFrame->Release();

		Transform* transform = &GetEntity()->m_Transform;

		transform->size.x = bmp->GetSize().width;
		transform->size.y = bmp->GetSize().height;
		if (!m_Isometric)
		{
			transform->position.x += transform->size.x / 2;
			transform->position.y += transform->size.y / 2;
		}
		else
		{
			// in isometric mode the centre point of a sprite is on the bottom
			transform->position.x += transform->size.x / 2;
			//transform->position.y += transform->size.y;
		}
		transform->layer = layer;
		transform->scale = { 1.0f, 1.0f };
		m_Active = true;

		GetEntity()->GetManager()->AddDrawableEntity(this, m_Layer, m_Isometric);
	}

	bool cRenderComponent::Update(float32 updateTime)
	{
		sMessage msg;
		while (Messenger.FetchMsg(GetUID(), &msg))
		{
			switch (msg.type)
			{
			case Msg_Die:
				return false;
				break;
			}
		}
		return true;
	}

	bool cRenderComponent::Render(Graphics* gfx)
	{
		Transform* transform = &GetEntity()->m_Transform;
		D2D_RECT_F src;
		if (transform->isAnimated)
		{	
			src = D2D1::RectF(
				(float)((transform->animationFrame.x / transform->spriteTable.x) * transform->size.x),
				(float)((transform->animationFrame.y + transform->animationFrame.y % transform->spriteTable.y) * transform->size.y),
				(float)((transform->animationFrame.x / transform->spriteTable.x) * transform->size.x) + transform->size.x,
				(float)((transform->animationFrame.y + transform->animationFrame.y % transform->spriteTable.y) * transform->size.y) + transform->size.y);
		}
		else
		{
			src = D2D1::RectF(0.0f, 0.0f, transform->size.x, transform->size.y);
		}
		
		D2D_RECT_F dest;
		if (m_Interface)	// if this sprite belongs to the user interface do not move it according to the camera
		{
			dest = D2D1::RectF(
				transform->position.x - transform->size.x / 2,
				transform->position.y - transform->size.y / 2,
				transform->position.x + transform->size.x / 2,
				transform->position.y + transform->size.y / 2);
		}
		else
		{
			dest = D2D1::RectF(
				gfx->GetCamera()->GetPosition().x + transform->position.x - transform->size.x / 2,
				gfx->GetCamera()->GetPosition().y + transform->position.y - transform->size.y / 2,
				gfx->GetCamera()->GetPosition().x + transform->position.x + transform->size.x / 2,
				gfx->GetCamera()->GetPosition().y + transform->position.y + transform->size.y / 2);
		}

		D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(transform->rotation, D2D1::Point2F(transform->position.x, transform->position.y));
		D2D1::Matrix3x2F matScale = D2D1::Matrix3x2F::Scale(transform->scale.x, transform->scale.y, D2D1::Point2F(transform->position.x, transform->position.y));

		gfx->GetContext()->SetTransform(rotation * matScale);

		gfx->GetContext()->DrawBitmap(bmp,
			dest,
			1.0f,
			D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
			src);

		gfx->GetContext()->SetTransform(D2D1::Matrix3x2F::Identity());

		return true;
	}


}