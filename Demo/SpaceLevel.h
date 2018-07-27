#pragma once
#include "Game.h"
#include "Input.h"
#include "TextRender.h"
#include "Sound.h"

using namespace Tijss;
using namespace Tijss::Audio;

class cPlayer
{
private:
	cEntity * m_ShipSprite;
	cEntity * m_ProjectileSprite;
	cSound	* m_ShootSound;
	bool m_Fired;
	float32 m_Speed;
public:
	void Init(cEntity * ship, cEntity * projectile, float32 speed);
	void UserInput(float32 updateTime);
};

class cNPC
{
private:
	cEntity * m_ShipSprite;
	cEntity * m_ProjectileSprite;
	bool m_Fired;
	cSound	* m_ShootSound;

public:
	cNPC(cEntity * ship, cEntity * projectile);
	void Update();
};

class SpaceLevel : public GameLevel
{
private:
	cPlayer m_PlayerClass;
	cNPC* m_NPCs[7];
public:
	SpaceLevel() : GameLevel("Media//")
	{
		float testingConstructor = 5 / 10;
	};
	void Load() override;
	void Unload() override;
	void Render() override;
	bool Update(double timeTotal, double timeDelta) override;
};
