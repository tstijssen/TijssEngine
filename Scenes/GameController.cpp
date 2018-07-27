#include "GameController.h"

GameLevel* GameController::currentLevel;
bool GameController::Loading;
HPTimer* GameController::hpTimer;

void GameController::Init()
{
	Loading = true;
	currentLevel = 0;
	hpTimer = new HPTimer;
}

void GameController::LoadInitialLevel(GameLevel* level)
{
	Loading = true;
	currentLevel = level;
	currentLevel->Load();
	Loading = false;
}
void GameController::SwitchLevel(GameLevel* level)
{
	Loading = true;
	currentLevel->Unload();
	level->Load();
	delete currentLevel;
	currentLevel = level;
	Loading = false;
}

void GameController::Render()
{
	if (Loading) return;
	currentLevel->Render();
}
bool GameController::Update()
{
	if (Loading) return true;
	hpTimer->Update();
	return currentLevel->Update(hpTimer->GetTimeTotal(), hpTimer->GetTimeDelta());
}