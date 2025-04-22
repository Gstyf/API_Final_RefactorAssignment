#pragma once
#include "codeanalysis/warnings.h"
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

//TODO: Move fields to private, use setters and getters
class Projectile
{
public:
	Projectile(Vector2 spawnPos, float direction) noexcept : 
		position(spawnPos), speed(direction) {}

	Vector2 position = { 0,0 };
	float speed = 0;
	bool active = true;

	void Update() noexcept;
	void Render(const MyTexture& texture) const noexcept;
	Rectangle GetRect(const MyTexture& texture) const noexcept;
};