#include "Wall.hpp"
#include "Constants.hpp"

void Wall::Update() noexcept
{
	// set walls as inactive when out of health
	if (health < 1)
	{
		active = false;
	}
}

void Wall::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(),
		static_cast<int>(position.y) - texture.HeightHalf(),
		WHITE);
	DrawText(TextFormat("%i", health), static_cast<int>(position.x) - wallHealthOffsetX,
		static_cast<int>(position.y), 40, RED);
}

Rectangle Wall::GetRect(const MyTexture& texture) const noexcept
{
	return Rectangle{ position.x - texture.WidthHalff(), position.y - texture.HeightHalff(),
						texture.Widthf(), texture.Heightf() };
}

