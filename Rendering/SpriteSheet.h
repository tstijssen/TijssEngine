#pragma once

#include <wincodec.h>
#include <d2d1_3.h>

#include "TypeDefines.h"
#include "Graphics.h"
#include "ThreadPool.h"

namespace CollisionType
{
	enum Type { None, Circle, Box, Point };
}

struct AnimationData
{
	bool animated;		// is sprite animated or not
	int frameColumns;	// number of columns in the spritesheet
	int frameRows;		// number of rows in the spritesheet
	sInt2 frameIndex;
	sInt2 startFrame;
	sInt2 endFrame;

	float frameTimer;	// countdown to next frame
	float animationDelay; // time in seconds for next frame
	int spritesAccross;
	int spritesDown;
	bool animationFinished;	// whether this frame the animation loops back
	bool paused;

	void SelectAnimation(sInt2 start, sInt2 end);
	void SetSpeed(float speed);
};

struct Transform
{
	sFloat2 prevPosition;
	sFloat2 position;
	sFloat2 size;
	sFloat2 origin;
	sFloat2 scale;
	float radius;
	float rotation;
	bool *layerMoved;

	void Scale(float x, float y);
	void Scale(float s);
	void Scale(sFloat2 scale);

	void Rotate(float angle);
	void SetRotation(float rotation);

	void Move(int x, int y);
	void Move(sFloat2 vector);

	void SetPosition(sFloat2 position);
	void SetPosition(int x, int y);
};

class SpriteBatch
{

public:
	SpriteBatch(Graphics* gfx);
	void AddSprites(string bitmap, int32 spriteNum, sFloat2 position, const D2D1_RECT_U* sources = NULL);
	void Clear();
	int32 GetNumSprites();
	void SetSprites();
	void SetSelection(int32 first, int32 last);
	void Draw(Graphics* gfx);

private:
	//friend class SpriteManager;
	Graphics * m_GFX;
	ID2D1SpriteBatch * m_Batch;
	ID2D1Bitmap1* m_Bitmap;
	sInt2 m_Selection;
	bool m_Active;
	sFloat2 position;
	sFloat2 size;
};

class SpriteSheet
{
	friend struct AnimationData;
	friend struct Transform;
	friend class SpriteManager;
	ID2D1Bitmap1* bmp;

	int managerIndex;
	bool isometric;

	void Init();
	void Draw(Graphics* gfx, double frameTime);
	SpriteSheet(CollisionType::Type collider, sFloat2 position, bool *layerMovedVar, bool isIsomtric);
	~SpriteSheet();
public:
	// public variables
	CollisionType::Type collider;
	SpriteSheet* other;

	AnimationData animation;
	Transform transform;
	void MakeAnimated(int colFrames, int rowFrames, float animspeed, int width, int height);

	int layer;
	bool active;
	string tag;
};

class StaticSprite : public SpriteSheet 
{

};

class DynamicSprite : public SpriteSheet
{

};