#pragma once
#include "codeanalysis/warnings.h"
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"

//TODO: Move fields to private, use setters and getters
class Player
{
public:
	explicit Player(Vector2 startPos) noexcept : position(startPos) {};

	Vector2 position = {};
	float speed = 7;
	float player_base_height = 100.0f;
	float radius = 50;
	int lives = 3;
	int activeTexture = 0;
	float timer = 0;

	void Update() noexcept;
	void Render(const MyTexture& texture) const noexcept;
	Rectangle GetRect(const MyTexture& texture) const noexcept;
	float GetMovement() const noexcept;
};
