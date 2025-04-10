#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include <string_view>
#include <format>
#include <stdexcept>
#include <print>

class MyTexture final {
	Texture2D myTex = {};
public:
	explicit MyTexture(std::string_view path)
	{
		myTex = LoadTexture(path.data());
		if (myTex.id <= 0)
		{
			throw std::runtime_error(std::format("Failed to load image at {}", path));
		}
	};
	MyTexture(const MyTexture&) = delete;
	MyTexture(MyTexture&&) = delete;
	MyTexture& operator=(const MyTexture&) = delete;
	MyTexture& operator=(MyTexture&&) = delete;
	~MyTexture()
	{
		UnloadTexture(myTex);
	}

	Texture2D GetTexture() const noexcept
	{
		return myTex;
	}
};
