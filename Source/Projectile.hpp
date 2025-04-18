#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

class Projectile
{
public:
	Projectile(Vector2 spawnPos, int direction) noexcept : position(spawnPos), speed(direction) {}

	Vector2 position = { 0,0 };
	int speed = 15;
	bool active = true;

	void Update();

	void Render(const MyTexture& texture);
};