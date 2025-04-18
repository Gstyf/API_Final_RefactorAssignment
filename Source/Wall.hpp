#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

class Wall
{
public:
	explicit Wall(Vector2 spawnPos) : position(spawnPos) {}
	Vector2 position;
	bool active = true;
	int health = 50;
	int radius = 60;

	void Render(const MyTexture& texture);
	void Update();
};
