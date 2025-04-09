#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include <string>
#include <format>
#include <stdexcept>
#include <print>

class MyTexture final {
public:
	Texture2D myTex = {};
	//TODO: Move back the throw into the constructor! Can't be done if textures are in the same class as window because window needs to exist FIRST!
	explicit MyTexture() noexcept = default;
	MyTexture(const MyTexture&) = delete;
	MyTexture(MyTexture&&) = delete;
	MyTexture& operator=(const MyTexture&) = delete;
	MyTexture& operator=(MyTexture&&) = delete;
	~MyTexture()
	{
		UnloadTexture(myTex);
	}

	void LoadMyTexture(std::string_view path)
	{
		myTex = LoadTexture(path.data());
		if (myTex.id <= 0)
		{
			throw std::runtime_error(std::format("Failed to load image at {}", path));
		}
	}

	Texture2D GetTexture() const noexcept
	{
		return myTex;
	}
};
