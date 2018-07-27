#pragma once

#include "Game.h"
#include "HPTimer.h"

class GameController
{
	GameController() {};
	static GameLevel* currentLevel;

	static HPTimer* hpTimer;
public:
	static bool Loading;

	static void Init();

	static void LoadInitialLevel(GameLevel* level);
	static void SwitchLevel(GameLevel* level);

	static void Render();
	static bool Update();
};