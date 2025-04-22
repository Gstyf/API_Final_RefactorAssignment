#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include <string_view>
#include <format>
#include <stdexcept>
#include <print>

class MyTexture {
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

	int Width() const noexcept
	{
		return myTex.width;
	}

	float Widthf() const noexcept
	{
		return static_cast<float>(myTex.width);
	}

	int WidthHalf() const noexcept
	{
		return myTex.width / 2;
	}

	float WidthHalff() const noexcept
	{
		return static_cast<float>(myTex.width / 2);
	}

	int Height() const noexcept
	{
		return myTex.height;
	}

	float Heightf() const noexcept
	{
		return static_cast<float>(myTex.height);
	}

	int HeightHalf() const noexcept
	{
		return myTex.height / 2;
	}

	float HeightHalff() const noexcept
	{
		return static_cast<float>(myTex.height / 2);
	}

	Rectangle GetTextureRectangle() const noexcept
	{
		return Rectangle{ 0,0, Widthf(), Heightf() };
	}
};
