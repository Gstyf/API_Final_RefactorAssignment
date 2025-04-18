#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable: ALL_CODE_ANALYSIS_WARNINGS )
#include "raylib.h"
#pragma warning(pop)
#include "Constants.hpp"
#include <stdexcept>

class MyWindow {
public:
	 MyWindow() {
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