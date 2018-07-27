#pragma once
#include "Graphics.h"
#include "EntityManager.h"

class GameLevel
{
protected:
	static Graphics* gfx;
	cEntityManager* EntityManager;
public:
	static void Init(Graphics* graphics)
	{
		gfx = graphics;
	}

	GameLevel(const string& mediaPath)
	{
		EntityManager = new cEntityManager(mediaPath);
	}

	virtual void Load() = 0;
	virtual void Unload() = 0;
	virtual void Render() = 0;
	virtual bool Update(double timeTotal, double timeDelta) = 0;

};