#pragma once
#include "SpriteSheet.h"
#include "Multithreading.h"

class SpriteManager
{
public:
	SpriteManager(Graphics* gfx);
	~SpriteManager();

	// no built in collision resolutions
	SpriteSheet* DetectCollisions(SpriteSheet* sprite);
	SpriteSheet* DetectMouseCollisions(sInt2 mousePos, int32 layer);
	bool OnMouse(sInt2 mousePos, SpriteSheet* sprite);

	SpriteSheet* AddSprite(string fileName, int layer = 0, CollisionType::Type collider = CollisionType::None, sFloat2 position = { 0,0 }, bool isIsometric = false);

	static void FolderInit(string spriteFolderPath)
	{
		m_FolderPath = spriteFolderPath + "\\";
	}

	void Draw(double frameTime);
	void RemoveSprite(int layer);

private:
	// layerstest
	struct spriteLayer
	{
		std::vector<SpriteSheet*> sprites;
		ID2D1Layer * renderLayer = NULL;
		bool active;
		bool isIsomentric;
		bool hasMoved;
	};

	multiThread::MultiThreader threader;

	std::vector<spriteLayer> WorldSprites;
	static string m_FolderPath;
	Graphics* gfx;
};