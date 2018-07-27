#include "SpriteSheet.h"
#include <algorithm>

template <typename T>
T clip(const T& n, const T& lower, const T& upper) {
	return (std::max)(lower, (std::min)(n, upper));
}

SpriteBatch::SpriteBatch(Graphics* gfx)
{
	gfx->GetContext()->CreateSpriteBatch(&m_Batch);
	m_GFX = gfx;
}

void SpriteBatch::AddSprites(string fileName, int32 spriteNum, sFloat2 position, const D2D1_RECT_U* sources)
{
	HRESULT hr;
	m_Selection.x = 1;
	m_Selection.y = spriteNum;
	string filePath = "Media\\" + fileName;
	std::wstring widestr = std::wstring(filePath.begin(), filePath.end());


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

	hr = m_GFX->GetContext()->CreateBitmapFromWicBitmap(
		wicConverter,
		NULL,
		&m_Bitmap
	);
	this->position = position;
	size.x = m_Bitmap->GetSize().width;
	size.y = m_Bitmap->GetSize().height;
	this->position.x += size.x / 2;
	this->position.y += size.y / 2;
	D2D1_RECT_U src = D2D1::RectU(0.0f, 0.0f, size.x, size.y);

	D2D1_RECT_F dest = D2D1::RectF(
		this->position.x - size.x / 2, this->position.y - size.y / 2,
		this->position.x + size.x / 2, this->position.y + size.y / 2);
	
	m_Batch->AddSprites(1, &dest, &src);

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();
}

void SpriteBatch::Draw(Graphics* gfx)
{
	gfx->GetContext()->DrawSpriteBatch(m_Batch, m_Selection.x, m_Selection.y, m_Bitmap);

	D2D_RECT_F src = D2D1::RectF(0.0f, 0.0f, size.x, size.y);

	D2D_RECT_F dest = D2D1::RectF(
		position.x - size.x / 2, position.y - size.y / 2,
		position.x + size.x / 2, position.y + size.y / 2);

	gfx->GetContext()->DrawBitmap(m_Bitmap,
		dest,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		src);
}

void SpriteBatch::Clear()
{
	m_Batch->Clear();
	
}

int32 SpriteBatch::GetNumSprites()
{
	return m_Batch->GetSpriteCount();
}

void SpriteBatch::SetSelection(int32 first, int32 last)
{
	m_Selection.x = first;
	m_Selection.y = last;
}

void SpriteBatch::SetSprites()
{

}


SpriteSheet::SpriteSheet(CollisionType::Type collider, sFloat2 position, bool *layerMovedVar, bool isIsometric)
{
	this->transform.position.x = position.x;
	this->transform.position.y = position.y;
	this->transform.layerMoved = layerMovedVar;
	this->collider = collider;
	this->isometric = isIsometric;
	other = NULL;
}

void SpriteSheet::Init()
{
	transform.size.x = bmp->GetSize().width;
	transform.size.y = bmp->GetSize().height;
	transform.position.x += transform.size.x / 2;
	transform.position.y += transform.size.y / 2;
	animation.spritesAccross = 1;
	transform.scale = { 1.0f, 1.0f };
	transform.radius = transform.size.x / 2;
	active = true;
	tag = "";
}

SpriteSheet::~SpriteSheet()
{
	if (bmp)
	{
		bmp->Release();
	}
}

void SpriteSheet::Draw(Graphics* gfx, double frameTime)
{
	// source depends on animated
	D2D_RECT_F src;
	if (animation.animated)
	{
		if (animation.frameTimer > 0.0f && !animation.paused)
		{
			animation.frameTimer -= frameTime;
		}
		else if (!animation.paused)
		{
			// advance column frame
			animation.frameIndex.x = (1 + animation.frameIndex.x);
			if (animation.frameIndex.x >= animation.endFrame.x)
			{
				animation.frameIndex.x = animation.startFrame.x;
				animation.frameIndex.y = (1 + animation.frameIndex.y);
				if (animation.frameIndex.y >= animation.endFrame.y)
				{
					animation.frameIndex.y = animation.startFrame.y;
				}
			}
			animation.animationFinished = false;

			if (animation.endFrame == sInt2(animation.frameIndex.x + 1, animation.frameIndex.y))
			{
				animation.animationFinished = true;
			}

			//animation.frameIndex = animation.startFrame + (1 + animation.frameIndex) % animation.endFrame;
			animation.frameTimer = animation.animationDelay;	// reset timer
			
			// calculate row and column of spritesheet


		}

		src = D2D1::RectF(
			(float)((animation.frameIndex.x / animation.spritesAccross) * transform.size.x),
			(float)((animation.frameIndex.y + animation.frameIndex.y % animation.spritesDown) * transform.size.y),
			(float)((animation.frameIndex.x / animation.spritesAccross) * transform.size.x) + transform.size.x,
			(float)((animation.frameIndex.y + animation.frameIndex.y % animation.spritesDown) * transform.size.y) + transform.size.y);
	}
	else
	{
		src = D2D1::RectF(0.0f, 0.0f, transform.size.x, transform.size.y);
	}

	D2D_RECT_F dest;
	if (layer == 0)	// layer 0 is the UI layer, do not read camera movement
	{
		dest = D2D1::RectF(
			transform.position.x - transform.size.x / 2,
			transform.position.y - transform.size.y / 2,
			transform.position.x + transform.size.x / 2,
			transform.position.y + transform.size.y / 2);
	}
	else
	{
		dest = D2D1::RectF(
			gfx->GetCamera()->GetPosition().x + transform.position.x - transform.size.x / 2,
			gfx->GetCamera()->GetPosition().y + transform.position.y - transform.size.y / 2,
			gfx->GetCamera()->GetPosition().x + transform.position.x + transform.size.x / 2,
			gfx->GetCamera()->GetPosition().y + transform.position.y + transform.size.y / 2);
	}

	D2D1::Matrix3x2F rotation = D2D1::Matrix3x2F::Rotation(transform.rotation, D2D1::Point2F(transform.position.x, transform.position.y));
	D2D1::Matrix3x2F matScale = D2D1::Matrix3x2F::Scale(transform.scale.x, transform.scale.y, D2D1::Point2F(transform.position.x, transform.position.y));

	gfx->GetContext()->SetTransform(rotation * matScale);

	gfx->GetContext()->DrawBitmap(bmp,
		dest,
		1.0f,
		D2D1_BITMAP_INTERPOLATION_MODE::D2D1_BITMAP_INTERPOLATION_MODE_NEAREST_NEIGHBOR,
		src);

	gfx->GetContext()->SetTransform(D2D1::Matrix3x2F::Identity());

	*transform.layerMoved = false;
}

void AnimationData::SelectAnimation(sInt2 start, sInt2 end)
{
	startFrame = {clip<int>(start.x, 0, frameColumns - 1), clip<int>(start.y, 0, frameRows - 1)};
	endFrame = { clip<int>(end.x, 0, frameColumns + 1), clip<int>(end.y, 0, frameRows + 1) };
	frameIndex = startFrame;
}

void AnimationData::SetSpeed(float speed)
{
	frameTimer = speed;
}

void SpriteSheet::MakeAnimated(int colFrames, int rowFrames, float animspeed, int width, int height)
{
	animation.animated = true;
	animation.frameTimer = 0;
	animation.animationDelay = animspeed;
	animation.frameColumns = colFrames;
	animation.frameRows = rowFrames;
	animation.SelectAnimation({ 0,0 }, { colFrames, rowFrames });
	animation.spritesAccross = (int)bmp->GetSize().width / transform.size.x;
	animation.spritesDown = (int)bmp->GetSize().height / transform.size.y;

	transform.size.x = width;
	transform.size.y = height;
	transform.position.x = transform.size.x / 2;
	transform.position.y = transform.size.y / 2;
	transform.radius = transform.size.x / 2;
	*transform.layerMoved = true;
}

void Transform::Move(int x, int y)
{
	prevPosition = position;
	position.x += x;
	position.y += y;
	*layerMoved = true;
}

void Transform::Move(sFloat2 vector)
{
	prevPosition = position;
	position.x += vector.x;
	position.y += vector.y;
	*layerMoved = true;
}

void Transform::SetPosition(sFloat2 position)
{
	prevPosition = position;
	this->position = position;
	*layerMoved = true;
}

void Transform::SetPosition(int x, int y)
{
	prevPosition = position;
	position.x = x;
	position.y = y;
	*layerMoved = true;
}

void Transform::Rotate(float angle)
{
	this->rotation += angle;
}

void Transform::SetRotation(float rotation)
{
	this->rotation = rotation;
}

void Transform::Scale(float scalar)
{
	scale.x += scalar;
	scale.y += scalar;
	*layerMoved = true;
}

void Transform::Scale(float x, float y)
{
	scale.x += x;
	scale.y += y;
	*layerMoved = true;
}

void Transform::Scale(sFloat2 scale)
{
	scale.x += scale.x;
	scale.y += scale.y;
	*layerMoved = true;
}