
#pragma warning(push)
#pragma warning(disable: 26446) //gsl::at() warning
#pragma warning(disable: 26472) //arithmetic conversion warning
#include "game.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

void Game::Start() {
	constexpr float wall_distance = _screenWidthF / (wallCount + 1);
	Walls.reserve(wallCount);
	for (int i = 0; i < wallCount; i++) {
		Walls.emplace_back(Vector2{ wall_distance * (i + 1.0f), _screenHeightF - wallOffsetY });
	}
	SpawnAliens();
	score = 0;
	gameState = State::GAMEPLAY;
}

void Game::End() noexcept
{
	playerProjectiles.clear();
	enemyProjectiles.clear();
	Walls.clear();
	Aliens.clear();
	newHighScore = CheckNewHighScore();
	gameState = State::ENDSCREEN;
}

void Game::Continue() noexcept
{
	gameState = State::STARTSCREEN;
}

void Game::Run()
{
	while (!WindowShouldClose())    // Detect window close button or ESC key
	{
		// Main game loop
		Update();
	}
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE)) Start();
		Render();
		break;
	case State::GAMEPLAY:
		GameplayUpdate();
		break;
	case State::ENDSCREEN:
		EndscreenUpdate();
		Render();
		break;
	default:
		break;
	}
}

void Game::GameplayUpdate()
{

	player.Update();

	// Update background with offset
	background.Update(static_cast<int>(player.position.x) - (GetScreenWidth() / 2));

	//Spawn new aliens if aliens run out
	if (Aliens.size() < 1)
	{
		SpawnAliens();
	}

	for (Wall& w : Walls)
	{
		w.Update();
	}
	CheckIfGameOver();

	UpdateProjectiles();
	ResolveProjectileCollisions();
	Shoot();
	AlienShoot();

	Render(); //render first, otherwise projectiles are removed before we can see them collide

	RemoveDeadEntities();
}

void Game::CheckIfGameOver() noexcept
{
	//End game if player dies
	if (player.lives < 1)
	{
		End();
	}

	//Update Aliens and Check if they are past player
	for (Alien& a : Aliens)
	{
		a.Update();
		if (a.position.y > static_cast<float>(GetScreenHeight()) - player.player_base_height)
		{
			End();
		}
	}
	//or if the player hits the venerable Q key
	if (IsKeyReleased(KEY_Q))
	{
		End();
	}
}

void Game::UpdateProjectiles() noexcept
{
	for (Projectile& pp : playerProjectiles)
	{
		pp.Update();
	}
	for (Projectile& ep : enemyProjectiles)
	{
		ep.Update();
	}
}

void Game::ResolveProjectileCollisions() noexcept
{
	for (Projectile& p : playerProjectiles)
	{
		for (Alien& a : Aliens)
		{
			if (p.active && CheckCollisionRecs(a.GetRect(alienTexture), p.GetRect(shipTextures.front())))
			{
				// Set them as inactive, will be killed later
				p.active = false;
				a.active = false;
				score += 100;
			}
		}
		for (Wall& w : Walls)
		{
			if (CheckCollisionRecs(w.GetRect(wallTexture), p.GetRect(laserTexture)))
			{
				// Set as inactive, will be killed later
				p.active = false;
				w.health -= 1;
			}
		}
	}

	//ENEMY PROJECTILES HERE
	for (Projectile& e : enemyProjectiles)
	{
		if (CheckCollisionRecs(player.GetRect(shipTextures.front()), e.GetRect(laserTexture)))
		{
			e.active = false;
			player.lives -= 1;
		}
		for (Wall& w : Walls)
		{
			if (CheckCollisionRecs(w.GetRect(wallTexture), e.GetRect(laserTexture)))
			{
				// Set them as inactive, will be killed later
				e.active = false;
				w.health -= 1;
			}
		}
	}
}

void Game::Shoot()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		const Projectile newProjectile(
			{ player.position.x, player.position.y - projectileSpawnOffset }, playerProjectileSpeed);
		playerProjectiles.push_back(newProjectile);
	}
}

void Game::AlienShoot()
{
	//Aliens Shooting
	shootTimer += 1;
	if (shootTimer > 20) //three times per second
	{
		int randomAlienIndex = 0;

		//TODO: Find clever way to get size of array and size -1
		if (Aliens.size() > 1) //checking that we don't have an empty container!
		{
			randomAlienIndex = GetRandomValue(0, static_cast<int>(Aliens.size()) - 1);
		}
		const Projectile newProjectile({ Aliens[randomAlienIndex].position }, enemyProjectileSpeed);
		enemyProjectiles.push_back(newProjectile);
		shootTimer = 0;
	}
}

void Game::SpawnAliens()
{
	for (int row = 0; row < formationHeight; row++) {
		for (int col = 0; col < formationWidth; col++) {
			const Alien newAlien({ formationX + 450 + (col * alienSpacing) , formationY + (row * alienSpacing) });
			Aliens.push_back(newAlien);
		}
	}
}

void Game::RemoveDeadEntities()
{
	auto isDead = []( const auto& e ) { return !e.active; };
	playerProjectiles.erase(std::remove_if(playerProjectiles.begin(),
		playerProjectiles.end(), isDead), playerProjectiles.end());
	
	enemyProjectiles.erase(std::remove_if(enemyProjectiles.begin(),
		enemyProjectiles.end(), isDead), enemyProjectiles.end());
	
	Aliens.erase(std::remove_if(Aliens.begin(),
		Aliens.end(), isDead), Aliens.end());
	
	Walls.erase(std::remove_if(Walls.begin(),
		Walls.end(), isDead), Walls.end());
}

void Game::EndscreenUpdate()
{
	//Exit end screen
	if (IsKeyReleased(KEY_ENTER) && !newHighScore)
	{
		Continue();
	}

	if (newHighScore)
	{
		if (CheckCollisionPointRec(GetMousePosition(), textBox)) mouseOnText = true;
		else mouseOnText = false;

		if (mouseOnText)
		{
			// Set the window's cursor to the I-Beam
			SetMouseCursor(MOUSE_CURSOR_IBEAM);

			HandleKeyboardInput();
		}
		else SetMouseCursor(MOUSE_CURSOR_DEFAULT);

		if (mouseOnText)
		{
			framesCounter++;
		}
		else
		{
			framesCounter = 0;
		}

		// If the name is right length and enter is pressed, exit screen by setting 
		// highscore to false and add name + score to scoreboard
		if (letterCount > 0 && letterCount < 9 && IsKeyReleased(KEY_ENTER))
		{
			InsertNewHighScore(highscoreNameEntry);
			newHighScore = false;
		}
	}
}

void Game::HandleKeyboardInput()
{
	// Get char pressed on the queue
	int key = GetCharPressed();

	// Check if more characters have been pressed on the same frame
	while (key > 0)
	{
		[[gsl::suppress(26472, justification: "I KNOW! I'm not bothered. I'm not losing any data here!")]]
			{
				// NOTE: Only allow keys in range [32..125]
				if ((key >= 32) && (key <= 125) && (letterCount < 9))
				{
					highscoreNameEntry += static_cast<char>(key);
					letterCount++;
				}
				key = GetCharPressed();  // Check next character in the queue
			}
	}

	//Remove chars 
	if (IsKeyPressed(KEY_BACKSPACE) && letterCount > 0)
	{
		letterCount--;
		highscoreNameEntry.pop_back();
	}
}

void Game::Render()
{
	BeginDrawing();
	ClearBackground(BLACK);
	switch (gameState)
	{
	case State::STARTSCREEN:
		DrawText("SPACE INVADERS", 200, 100, 160, YELLOW);
		DrawText("PRESS SPACE TO BEGIN", 200, 350, 40, YELLOW);
		break;
	case State::GAMEPLAY:
		GamePlayDraw();
		break;
	case State::ENDSCREEN:
		EndgameDraw();
		break;
	default:
		//SHOULD NOT HAPPEN
		break;
	}
	EndDrawing();
}

void Game::GamePlayDraw() const noexcept
{
	background.Render(bgTexture);
	DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);

	for (const Wall& w : Walls)
	{
		w.Render(wallTexture);
	}

	for (const Projectile& p : playerProjectiles)
	{
		p.Render(laserTexture);
	}
	for (const Projectile& e : enemyProjectiles)
	{
		e.Render(laserTexture);
	}

	for (const Alien& a : Aliens)
	{
		a.Render(alienTexture);
	}

	player.Render(shipTextures[player.activeTexture]);
}

void Game::EndgameDraw()
{
	if (newHighScore)
	{
		DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

		// BELOW CODE IS FOR NAME INPUT RENDER
		DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

		DrawRectangleRec(textBox, LIGHTGRAY);
		if (mouseOnText)
		{
			// HOVER CONFIRMIATION
			DrawRectangleLines(static_cast<int>(textBox.x), static_cast<int>(textBox.y),
				static_cast<int>(textBox.width), static_cast<int>(textBox.height), RED);
		}
		else
		{
			DrawRectangleLines(static_cast<int>(textBox.x), static_cast<int>(textBox.y),
				static_cast<int>(textBox.width), static_cast<int>(textBox.height), DARKGRAY);
		}

		//Draw the name being typed out
		DrawText(highscoreNameEntry.data(), static_cast<int>(textBox.x) + 5,
			static_cast<int>(textBox.y) + 8, 40, MAROON);

		//Draw the text explaining how many characters are used
		DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

		HandleMouseOverNameInput();
		// Explain how to continue when name is input
		if (letterCount > 0 && letterCount < 9)
		{
			DrawText("PRESS ENTER TO CONTINUE", 600, 800, 40, YELLOW);
		}
	}
	else {
		// If no highscore or name is entered, show scoreboard and call it a day
		DrawText("PRESS ENTER TO CONTINUE", 600, 200, 40, YELLOW);

		DrawText("LEADERBOARD", 50, 100, 40, YELLOW);

		int highscoreOffset = 0;
		for (PlayerData pd : Leaderboard)
		{
			DrawText(pd.name.data(), 50, 140 + (highscoreOffset * 40), 40, YELLOW);
			DrawText(TextFormat("%i", pd.score), 350, 140 + (highscoreOffset * 40), 40, YELLOW);
			++highscoreOffset;
		}
	}
}

void Game::HandleMouseOverNameInput() noexcept
{
	if (mouseOnText)
	{
		if (letterCount < 9)
		{
			// Draw blinking underscore char
			if (((framesCounter / 20) % 2) == 0)
			{
				DrawText("_", static_cast<int>(textBox.x) + 8 + MeasureText(highscoreNameEntry.data(), 40),
					static_cast<int>(textBox.y) + 12, 40, MAROON);
			}
		}
		else
		{
			//Name needs to be shorter
			DrawText("Press BACKSPACE to delete chars...", 600, 650, 20, YELLOW);
		}
	}
}

bool Game::CheckNewHighScore() noexcept
{
	if (score > Leaderboard.back().score)
	{
		return true;
	}
	else
	{
		return false;
	}
}

void Game::InsertNewHighScore(const std::string& tName)
{
	PlayerData newData{ tName, score };
	for (int i = 0; i < Leaderboard.size(); i++)
	{
		if (newData.score > Leaderboard[i].score)
		{
			Leaderboard.insert(Leaderboard.begin() + i, newData);
			Leaderboard.pop_back();
			return;
		}
	}
}

//BACKGROUND
void Background::Update(int offs) noexcept
{
	bgOffset = offs / 10;
}

void Background::Render(const MyTexture& texture) const noexcept
{
	DrawTexture(texture.GetTexture(),
		(GetScreenWidth() / 2) - texture.WidthHalf() - bgOffset, 
		0, WHITE);
}
#pragma warning(pop)
