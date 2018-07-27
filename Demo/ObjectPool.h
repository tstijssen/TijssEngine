#pragma once
#include"TypeDefines.h"

//class Bullet
//{
//	SpriteSheet* bulletSprite;
//	int32 speed;
//public:
//	bool inUse_;
//
//	void Create(SpriteSheet* sprite, int32 speed)
//	{
//		bulletSprite = sprite;
//		inUse_ = false;
//		this->speed = speed;
//	}
//
//	void Update(SpriteManager* manager)
//	{
//		bulletSprite->MoveY(-speed);
//		SpriteSheet* collider = manager->DetectCollisions(bulletSprite);
//		if (bulletSprite->position.y < 0 || bulletSprite->position.y > 1100 || collider != NULL)
//		{
//			if(collider != NULL)
//				collider->active = false;
//			bulletSprite->active = false;
//			inUse_ = false;
//		}
//	}
//	void Activate(sFloat2 pos)
//	{
//		bulletSprite->active = true;
//		bulletSprite->position = pos;
//		inUse_ = true;
//	}
//};
//
//class BulletPool
//{
//public:
//	BulletPool(SpriteManager* manager, string bulletFilename, int32 bulletSpeed, int32 bulletLayer)
//	{
//		this->manager = manager;
//		for (int i = 0; i < POOL_SIZE; ++i)
//		{
//			SpriteSheet* newSprite = manager->AddSprite(bulletFilename, bulletLayer, CollisionType::Box);
//			newSprite->active = false;
//			pool_[i].Create(newSprite, bulletSpeed);
//		}
//	}
//
//	bool ActivateBullet(sFloat2 pos)
//	{
//		for (int i = 0; i < POOL_SIZE; i++)
//		{
//			if (!pool_[i].inUse_)
//			{
//				pool_[i].Activate(pos);
//				return true;
//			}
//		}
//		return false;
//	}
//
//	void Update()
//	{
//		for (int i = 0; i < POOL_SIZE; i++)
//		{
//			if (pool_[i].inUse_)
//			{
//				pool_[i].Update(manager);
//			}
//		}
//	}
//private:
//	static const int POOL_SIZE = 10;
//
//	SpriteManager* manager;
//	Bullet pool_[POOL_SIZE];
//};