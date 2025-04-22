#include "Alien.hpp"
#include "Constants.hpp"

void Alien::Update() noexcept
{
	position.x += speed;
	if (position.x >= _screenWidthF || position.x <= 0)
	{
		speed *= -1;
		position.y += alienForwardMovement;
	}
}

void Alien::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(),
		static_cast<int>(position.y) - texture.HeightHalf(), WHITE);
}

Rectangle Alien::GetRect(const MyTexture& texture) const noexcept
{
	return Rectangle{ position.x - texture.WidthHalff(), position.y - texture.HeightHalff(),
						texture.Widthf(), texture.Heightf() };
}