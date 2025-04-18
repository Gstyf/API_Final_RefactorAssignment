#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

//TODO: Move fields to private, use setters and getters
class Player
{
public:
	explicit Player(float xpos) noexcept : x_pos(xpos) {};

	float x_pos = 0;
	float speed = 7;
	float player_base_height = 100.0f;
	float radius = 50;
	int lives = 3;
	int direction = 0;
	int activeTexture = 0;
	float timer = 0;
	Vector2 sizeOffset = { 0,0 };
	Rectangle aabb = { 0,0,0,0 };

	void Render(const MyTexture& texture);
	void Update();
};
