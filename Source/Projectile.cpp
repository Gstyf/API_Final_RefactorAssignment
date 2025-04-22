#include "Projectile.hpp"

void Projectile::Update() noexcept
{
	position.y -= speed;

	if (position.y < 0 || position.y > 1500)
	{
		active = false;
	}
}

void Projectile::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(),
		static_cast<int>(position.y) - texture.HeightHalf(), WHITE);
}

Rectangle Projectile::GetRect(const MyTexture& texture) const noexcept
{
	return Rectangle{ position.x - texture.WidthHalff(), position.y - texture.HeightHalff(),
						texture.Widthf(), texture.Heightf() };
}
