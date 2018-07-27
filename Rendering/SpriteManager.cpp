#include "SpriteManager.h"

using namespace nbsdx::concurrent;

string SpriteManager::m_FolderPath;

SpriteManager::SpriteManager(Graphics* gfx)
{
	this->gfx = gfx;
	// initial sprite layer
	spriteLayer backgroundLayer;
	backgroundLayer.isIsomentric = true;
	WorldSprites.push_back(backgroundLayer);
}

SpriteManager::~SpriteManager()
{
	std::vector<spriteLayer>::iterator layerIter;

	for (layerIter = WorldSprites.begin(); layerIter != WorldSprites.end(); ++layerIter)
	{
		if ((*layerIter).renderLayer)
		{
			(*layerIter).renderLayer->Release();
		}
		std::vector<SpriteSheet*>::iterator spriteIter;
		for (spriteIter = (*layerIter).sprites.begin(); spriteIter != (*layerIter).sprites.end(); ++spriteIter)
		{
			if ((*spriteIter))
			{
				delete (*spriteIter);
			}
		}
	}

}

SpriteSheet* SpriteManager::AddSprite(string fileName, int layer, CollisionType::Type collider, sFloat2 position, bool isIsometric)
{
	// add new layers?
	if (layer >= WorldSprites.size())
	{
		for (int i = WorldSprites.size(); i <= layer; ++i)
		{
			spriteLayer newLayer;
			WorldSprites.push_back(newLayer);
		}
	}

	SpriteSheet* newSprite = new SpriteSheet(collider, position, &WorldSprites[layer].hasMoved, isIsometric);
	newSprite->bmp = NULL;

	HRESULT hr;
	string filePath = m_FolderPath + fileName;
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

	hr = gfx->GetContext()->CreateBitmapFromWicBitmap(
		wicConverter,
		NULL,
		&newSprite->bmp
	);

	if (wicFactory) wicFactory->Release();
	if (wicDecoder) wicDecoder->Release();
	if (wicConverter) wicConverter->Release();
	if (wicFrame) wicFrame->Release();

	newSprite->layer = layer;
	newSprite->Init();
	hr = gfx->GetContext()->CreateLayer(NULL, &WorldSprites[layer].renderLayer);
	WorldSprites[layer].active = true;
	WorldSprites[layer].isIsomentric = true;
	WorldSprites[layer].sprites.push_back(newSprite);
	return newSprite;
}

struct isometric_sort
{
	inline bool operator() (const SpriteSheet* spriteA, const SpriteSheet* spriteB)
	{
		return (spriteA->transform.position.y + (spriteA->transform.size.y / 2) < spriteB->transform.position.y + (spriteB->transform.size.y / 2));
	}
};

void SpriteManager::Draw(double frameTime)
{
	///////////////////MULTITHREADING...ONE LAYER EACH OR ONE SPRITE EACH?///////////////////////

	std::vector<spriteLayer>::iterator layerIter = WorldSprites.end();

	// render layers up to 0, 0 is closest to camera (ui layer)
	while (layerIter != WorldSprites.begin())
	{
		--layerIter;

		if ((*layerIter).active)
		{		
			if ((*layerIter).hasMoved)
			{
				// sort sprites vector
				std::sort((*layerIter).sprites.begin(), (*layerIter).sprites.end(), isometric_sort());
			}
			gfx->GetContext()->PushLayer(
				D2D1::LayerParameters(D2D1::InfiniteRect(),
					NULL,
					D2D1_ANTIALIAS_MODE_PER_PRIMITIVE,
					D2D1::IdentityMatrix(),
					1.0f,
					(ID2D1Brush*)0,
					D2D1_LAYER_OPTIONS_NONE
				),
				(*layerIter).renderLayer
			);

			std::vector<SpriteSheet*>::iterator spriteIter;
			for (spriteIter = (*layerIter).sprites.begin(); spriteIter != (*layerIter).sprites.end(); ++spriteIter)
			{
				if ((*spriteIter)->active)
				{
					(*spriteIter)->Draw(gfx, frameTime);
				}
			}

			gfx->GetContext()->PopLayer();
			(*layerIter).hasMoved = false;
		}	
	}
}

void SpriteManager::RemoveSprite(int index)
{
}

SpriteSheet* SpriteManager::DetectCollisions(SpriteSheet* sprite)
{
	float roundedSpritesNum = ceil(1.0f * WorldSprites[sprite->layer].sprites.size() / (threader.m_NumWorkers + 1));
	uint32_t SpritesPerThread = roundedSpritesNum;
	uint32_t threadSelection = 0;
	for (uint32_t t = 0; t < threader.m_NumWorkers; ++t)
	{
		auto& work = threader.m_CollisionWorkers[t].second;

		work.sprite = sprite;
		work.collidersStart = WorldSprites[sprite->layer].sprites.begin() + threadSelection;
		work.collidersEnd = WorldSprites[sprite->layer].sprites.begin() + (threadSelection + SpritesPerThread);

		auto& workerThread = threader.m_CollisionWorkers[t].first;
		{
			std::unique_lock<std::mutex> lock(workerThread.lock);
			work.complete = false;
		}

		workerThread.workReady.notify_one();

		threadSelection += SpritesPerThread;
	}

	threader.SpriteCollisions(sprite, WorldSprites[sprite->layer].sprites.begin() + threadSelection, WorldSprites[sprite->layer].sprites.end());

	for (int t = 0; t < threader.m_NumWorkers; ++t)
	{
		auto& work = threader.m_CollisionWorkers[t].second;
		auto& workerThread = threader.m_CollisionWorkers[t].first;

		std::unique_lock<std::mutex> lock(workerThread.lock);
		workerThread.workReady.wait(lock, [&]() {return work.complete; });

	}

	return NULL;
}

SpriteSheet* SpriteManager::DetectMouseCollisions(sInt2 mousePos, int32 layer)
{
	for (std::vector<SpriteSheet*>::iterator it = WorldSprites[layer].sprites.begin(); it != WorldSprites[layer].sprites.end(); ++it)
	{
		if ((*it)->collider != CollisionType::None)
		{
			int bX;
			int bY;
			int bWidth;
			int bHeight;
			int bRadius;
			switch ((*it)->collider)
			{
			case CollisionType::Box:
			{
				bWidth = (*it)->transform.size.x;				// maxX
				bHeight = (*it)->transform.size.y;			// maxY
				bX = (*it)->transform.position.x - bWidth / 2;				// minX
				bY = (*it)->transform.position.y - bWidth / 2;				// minY
				if (mousePos.x > bX && mousePos.x < bWidth && mousePos.y > bY && mousePos.y < bHeight)
				{
					return (*it);
				}
				break;
			}
			case CollisionType::Circle:
			{
				bX = (*it)->transform.position.x;
				bY = (*it)->transform.position.y;
				bRadius = (*it)->transform.radius;
				int x = abs(bX - mousePos.x);
				int y = abs(bY - mousePos.y);
				int distance = sqrt(x * x + y * y);

				if (distance < bRadius)
				{
					return (*it);
				}
				break;
			}
			case CollisionType::Point:
			{
				bX = (*it)->transform.position.x;
				bY = (*it)->transform.position.y;

				if (mousePos.x == bX && mousePos.y == bY)
				{
					return (*it);
				}
				break;
			}
			default:
				break;
			}
		}
	}
	return NULL;
}

bool SpriteManager::OnMouse(sInt2 mousePos, SpriteSheet* sprite)
{
	int bX;
	int bY;
	int bWidth;
	int bHeight;
	int bRadius;
	switch (sprite->collider)
	{
	case CollisionType::Box:
	{
		bWidth = sprite->transform.position.x + sprite->transform.size.x / 2;				// maxX
		bHeight = sprite->transform.position.y + sprite->transform.size.y / 2;			// maxY

		bX = sprite->transform.position.x - sprite->transform.size.x / 2;				// minX
		bY = sprite->transform.position.y - sprite->transform.size.y / 2;				// minY
		if (mousePos.x > bX && mousePos.x < bWidth && mousePos.y > bY && mousePos.y < bHeight)
		{
			return true;
		}
		break;
	}
	case CollisionType::Circle:
	{
		bX = sprite->transform.position.x;
		bY = sprite->transform.position.y;
		bRadius = sprite->transform.radius;
		int x = abs(bX - mousePos.x);
		int y = abs(bY - mousePos.y);
		int distance = sqrt(x * x + y * y);

		if (distance < bRadius)
		{
			return true;
		}
		break;
	}
	case CollisionType::Point:
	{
		bX = sprite->transform.position.x;
		bY = sprite->transform.position.y;

		if (mousePos.x == bX && mousePos.y == bY)
		{
			return true;
		}
		break;
	}
	default:
		break;
	}
	return false;
}