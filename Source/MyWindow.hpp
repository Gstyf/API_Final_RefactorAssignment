#pragma once
#include "raylib.h"
#include "Constants.hpp"
#include <stdexcept>

class MyWindow {
public:
	explicit MyWindow() {
		InitWindow(_WINDOW_WIDTH, _WINDOW_HEIGHT, _WINDOW_TITLE.data());
		if (!IsWindowReady())
		{
			throw std::runtime_error("Failed to open game window!");
		}
		SetTargetFPS(60);
	}
	MyWindow(const MyWindow&) = delete;
	MyWindow(MyWindow&&) = delete;
	MyWindow& operator=(const MyWindow&) = delete;
	MyWindow& operator=(MyWindow&&) = delete;
	~MyWindow()
	{
		CloseWindow();
	}
};