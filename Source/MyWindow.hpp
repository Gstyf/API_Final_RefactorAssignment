#pragma once
#include "raylib.h"
#include <stdexcept>

class MyWindow {
public:
	explicit MyWindow() {
		InitWindow(1800, 900, "SPACE INVADER");
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