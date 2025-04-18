#include "Projectile.hpp"

void Projectile::Update()
{
	position.y -= speed;

	if (position.y < 0 || position.y > 1500)
	{
		active = false;
	}
}

void Projectile::Render(const MyTexture& texture)
{
	//DrawCircle((int)position.x, (int)position.y, 10, RED);
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(),
		static_cast<int>(position.y) - texture.HeightHalf(), WHITE);
}