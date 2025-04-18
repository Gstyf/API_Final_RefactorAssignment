#include "Alien.hpp"

void Alien::Update() noexcept
{
	if (moveRight)
	{
		position.x += speed;

		if (position.x >= GetScreenWidth())
		{
			moveRight = false;
			position.y += 50;
		}
	}
	else
	{
		position.x -= speed;

		if (position.x <= 0)
		{
			moveRight = true;
			position.y += 50;
		}
	}
}

void Alien::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(),
		static_cast<int>(position.y) - texture.HeightHalf(), WHITE);
}
