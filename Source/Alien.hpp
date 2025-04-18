#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

class Alien
{
public:
	static constexpr Color color = WHITE; // IF THEY DON'T CHANGE, THEY CAN BE CONSTEXPR, but why do they need to store color?
	Vector2 position = { 0, 0 }; //Why do we have position twice?!
	int x = 0;
	int y = 0;
	static constexpr float radius = 30;
	bool active = true;
	bool moveRight = true;

	//EntityType type = EntityType::ENEMY;

	int speed = 2;

	void Update();
	void Render(const MyTexture& texture);
};