#pragma once
#include "Game.h"
#include "Input.h"
#include "TextRender.h"
#include "Sound.h"

using namespace Tijss;
using namespace Tijss::Audio;

class IsometricLevel : public GameLevel
{
private:
	cEntity * playerEntity;
	int32 windowHeight;
	int32 windowWidth;
public:
	IsometricLevel() : GameLevel("Media//")
	{
		float testingConstructor = 5 / 10;
	};
	void Load() override;
	void Unload() override;
	void Render() override;
	bool Update(double timeTotal, double timeDelta) override;
};