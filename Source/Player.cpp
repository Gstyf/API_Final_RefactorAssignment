#include "Player.hpp"

void Player::Update() noexcept
{
	position.x += speed * GetMovement();

	if (position.x < 0 + radius)
	{
		position.x = 0 + radius;
	}
	else if (position.x > GetScreenWidth() - radius)
	{
		position.x = GetScreenWidth() - radius;
	}
	//Determine frame for animation
	timer += GetFrameTime();

	if (timer > 0.4 && activeTexture == 2)
	{
		activeTexture = 0;
		timer = 0;
	}
	else if (timer > 0.4)
	{
		activeTexture++;
		timer = 0;
	}
}

void Player::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		static_cast<int>(position.x) - texture.WidthHalf(), 
		static_cast<int>(position.y) - texture.Height(), WHITE);
}

Rectangle Player::GetRect(const MyTexture& texture) const noexcept
{
	return Rectangle{ position.x - texture.WidthHalff(), position.y - texture.HeightHalff(),
					texture.Widthf(), texture.Heightf() };
}

float Player::GetMovement() const noexcept
{
	return static_cast<float>((IsKeyDown(KEY_RIGHT) - IsKeyDown(KEY_LEFT)));
}
