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
	//SAVE SCORE AND UPDATE SCOREBOARD
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
		Render();
	}
}

void Game::Update()
{
	switch (gameState)
	{
	case State::STARTSCREEN:
		if (IsKeyReleased(KEY_SPACE)) Start();
		break;
	case State::GAMEPLAY:
		GameplayUpdate();
		break;
	case State::ENDSCREEN:
		EndscreenUpdate();
		break;
	default:
		break;
	}
}


void Game::GameplayUpdate()
{
	if (IsKeyReleased(KEY_Q))
	{
		End();
	}
	CheckIfGameOver();

	player.Update();
	background.Update(static_cast<int>(player.position.x) - (GetScreenWidth() / 2));

	//Spawn new aliens if aliens run out
	if (Aliens.size() < 1)
	{
		SpawnAliens();
	}

	// Update background with offset

	for (Wall& w : Walls)
	{
		w.Update();
	}

	UpdateProjectiles();
	ResolveProjectileCollisions();
	Shoot();

	AlienShoot();

	// REMOVE INACTIVE/DEAD ENITITIES
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
			if (CheckCollisionRecs(a.GetRect(alienTexture), p.GetRect(shipTextures.front())))
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
				// Set them as inactive, will be killed later
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
	if (shootTimer > 59) //once per second
	{
		int randomAlienIndex = 0;

		//TODO: Find clever way to get size of array and size -1
		if (Aliens.size() > 1)
		{
			randomAlienIndex = GetRandomValue(0, static_cast<int>(std::ssize(Aliens) - 1));
		}

		//TODO: Consider a GetRandomFromContainer() with a crash if the container is empty (ASSERT) WE ARE already checking that the container isn't empty, so safe?
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
	//TODO: use lambdas remove_if to move dead ones to back, and then erase dead entities
	//TODO: use ranged-fors instead
	for (int i = 0; i < playerProjectiles.size(); i++)
	{
		if (playerProjectiles[i].active == false)
		{
			playerProjectiles.erase(playerProjectiles.begin() + i);
			// Prevent the loop from skipping an instance because of index changes, since all insances after
			// the killed objects are moved down in index. This is the same for all loops with similar function
			i--;
		}
	}
	for (int i = 0; i < enemyProjectiles.size(); i++)
	{
		if (enemyProjectiles[i].active == false)
		{
			enemyProjectiles.erase(enemyProjectiles.begin() + i);
			// Prevent the loop from skipping an instance because of index changes, since all insances after
			// the killed objects are moved down in index. This is the same for all loops with similar function
			i--;
		}
	}
	for (int i = 0; i < Aliens.size(); i++)
	{
		if (Aliens[i].active == false)
		{
			Aliens.erase(Aliens.begin() + i);
			i--;
		}
	}
	for (int i = 0; i < Walls.size(); i++)
	{
		if (Walls[i].active == false)
		{
			Walls.erase(Walls.begin() + i);
			i--;
		}
	}
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
		// NOTE: Only allow keys in range [32..125]
		if ((key >= 32) && (key <= 125) && (letterCount < 9))
		{
			highscoreNameEntry += static_cast<char>(key);
			letterCount++;
		}
		key = GetCharPressed();  // Check next character in the queue
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

			i = static_cast<int>(Leaderboard.size());

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
		(GetScreenWidth() / 2) - texture.WidthHalf() - bgOffset, 0, WHITE);
}


