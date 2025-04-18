#include "Wall.hpp"

void Wall::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(),
		static_cast<int>(position.y) - texture.HeightHalf(),
		WHITE);
	DrawText(TextFormat("%i", health), static_cast<int>(position.x) - 21,
		static_cast<int>(position.y) + 10, 40, RED);
}

void Wall::Update() noexcept
{
	// set walls as inactive when out of health
	if (health < 1)
	{
		active = false;
	}
}
