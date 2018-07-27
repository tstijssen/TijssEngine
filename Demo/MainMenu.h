#pragma once
#include "Game.h"
#include "Input.h"
#include "TextRender.h"
#include "Sound.h"

using namespace Tijss;
using namespace Tijss::Audio;

class MainMenu : public GameLevel
{
	double frameTime;
	cSound* MainMenuMusic;

	cEntityTemplate* buttonTemplate;
	TEntityUID* eSpaceShooterStartBtn;
	TEntityUID* eIsometricStartBtn;
	TEntityUID* eExitButton;
public:
	MainMenu() : GameLevel("Media//") {};
	void Load() override;
	void Unload() override;
	void Render() override;
	bool Update(double timeTotal, double timeDelta) override;
};