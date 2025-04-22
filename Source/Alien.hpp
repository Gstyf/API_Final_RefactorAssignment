#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

//TODO: Move fields to private, use setters and getters
class Alien
{
public:
	explicit Alien(Vector2 spawnPos) noexcept : position(spawnPos) {}
	
	Vector2 position = { 0, 0 };
	static constexpr float radius = 30;
	bool active = true;
	bool moveRight = true;

	float speed = 2;

	void Update() noexcept;
	void Render(const MyTexture& texture) const noexcept;
	Rectangle GetRect(const MyTexture& texture) const noexcept;
};