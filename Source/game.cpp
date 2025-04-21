#include "game.hpp"
#include <chrono>
#include <fstream>
#include <iostream>
#include <thread>
#include <vector>

void Game::Start()
{
	// creating walls 
	const auto window_width = static_cast<float>(GetScreenWidth());
	const float wall_distance = window_width / (wallCount + 1);
	for (int i = 0; i < wallCount; i++)
	{
		const Wall newWalls({ wall_distance * (i + 1), static_cast<float>(GetScreenHeight() - 250) });
		Walls.push_back(newWalls);
	}

	//creating aliens
	SpawnAliens();

	//reset score
	score = 0;
	gameState = State::GAMEPLAY;
}

void Game::End()
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
		GamePlayLogic();
		break;
	case State::ENDSCREEN:
		EndScreenLogic();
		break;
	default:
		break;
	}
}

//TODO: Refactor A LOT!
void Game::GamePlayLogic()
{
	//Code
	if (IsKeyReleased(KEY_Q))
	{
		End();
	}
	//Update Player
	player.Update();
	//End game if player dies
	if (player.lives < 1)
	{
		End();
	}

	//Update Aliens and Check if they are past player
	for (Alien& a : Aliens)
	{
		a.Update();
		if (a.position.y > GetScreenHeight() - player.player_base_height)
		{
			End();
		}
	}

	//Spawn new aliens if aliens run out
	if (Aliens.size() < 1)
	{
		SpawnAliens();
	}

	// Update background with offset
	background.Update( static_cast<int>(player.x_pos) - (GetScreenWidth() / 2) );//TODO: Magic number

	for (Wall& w : Walls)
	{
		w.Update();
	}

	//UPDATE PROJECTILE
	for (Projectile& pp : playerProjectiles)
	{
		pp.Update();
	}
	for (Projectile& ep : enemyProjectiles)
	{
		ep.Update();
	}

	//UPDATE PROJECTILE
	for (Projectile& p : playerProjectiles)
	{
		for (Alien& a : Aliens)
		{
			if (CheckCollisionRecs(
				{
					a.position.x - alienTexture.WidthHalff(),
					a.position.y - alienTexture.HeightHalff(),
					alienTexture.Widthf(), alienTexture.Heightf()
				},
						{
							p.position.x - laserTexture.WidthHalff(),
							p.position.y - laserTexture.HeightHalff(),
							laserTexture.Widthf(), laserTexture.Heightf()
						}))
			{
				// Kill!
				std::cout << "Hit! \n";
				// Set them as inactive, will be killed later
				p.active = false;
				a.active = false;
				score += 100;
			}
		}
		for (Wall& w : Walls)
		{
			if (CheckCollisionRecs(
				{
					w.position.x - wallTexture.WidthHalff(),
					w.position.y - wallTexture.HeightHalff(),
					wallTexture.Widthf(), wallTexture.Heightf()
				},
						{
							p.position.x - laserTexture.WidthHalff(),
							p.position.y - laserTexture.HeightHalff(),
							laserTexture.Widthf(), laserTexture.Heightf()
						})
				)
			{
				// Kill!
				std::cout << "Hit! \n";
				// Set them as inactive, will be killed later
				p.active = false;
				w.health -= 1;
			}
		}
	}

	//ENEMY PROJECTILES HERE
	for (Projectile& e : enemyProjectiles)
	{
		if (CheckCollisionRecs(
			{
				player.x_pos - shipTextures[player.activeTexture].WidthHalff(),
				GetScreenHeight() - shipTextures[player.activeTexture].Heightf(),
				shipTextures[player.activeTexture].Widthf(), shipTextures[player.activeTexture].Heightf()
			},
						{
							e.position.x - laserTexture.WidthHalff(),
							e.position.y - laserTexture.HeightHalff(),
							laserTexture.Widthf(), laserTexture.Heightf()
						})
			)
		{
			std::cout << "dead!\n";
			e.active = false;
			player.lives -= 1;
		}
		for (Wall& w : Walls)
		{
			if (CheckCollisionRecs(
				{
					w.position.x - wallTexture.WidthHalff(),
					w.position.y - wallTexture.HeightHalff(),
					wallTexture.Widthf(), wallTexture.Heightf()
				},
						{
							e.position.x - laserTexture.WidthHalff(),
							e.position.y - laserTexture.HeightHalff(),
							laserTexture.Widthf(), laserTexture.Heightf()
						})
				)
			{
				// Kill!
				std::cout << "Hit! \n";
				// Set them as inactive, will be killed later
				e.active = false;
				w.health -= 1;
			}
		}
	}

	//MAKE PROJECTILE
	Shoot();

	//TODO: consider making alien shooting more interesting and erratic. Now only one alient will shoot every 2 seconds, and it is random which one.
	//Aliens Shooting
	shootTimer += 1;
	if (shootTimer > 59) //once per second
	{
		int randomAlienIndex = 0;

		if (Aliens.size() > 1)
		{
			randomAlienIndex = GetRandomValue(0, static_cast<int>(Aliens.size()) - 1);
		}

		const Projectile newProjectile({ Aliens[randomAlienIndex].position }, enemyProjectileSpeed);
		enemyProjectiles.push_back(newProjectile);
		shootTimer = 0;
	}

	//TODO: refactor cleanup into own function. 
	//TODO: use ranged-fors instead
	// REMOVE INACTIVE/DEAD ENITITIES
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

void Game::EndScreenLogic()
{
	//Exit end screen
	if (IsKeyReleased(KEY_ENTER) && !newHighScore)
	{
		Continue();
	}

	//TODO: Refactor highscore screen into own function
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

//TODO: Refactor biggly, this is a long function (130 lines)
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

void Game::GamePlayDraw()
{
	//background render LEAVE THIS AT TOP
	background.Render(bgTexture);
	DrawText(TextFormat("Score: %i", score), 50, 20, 40, YELLOW);
	DrawText(TextFormat("Lives: %i", player.lives), 50, 70, 40, YELLOW);

	//player rendering 
	player.Render(shipTextures[player.activeTexture]);

	//wall rendering 
	for (Wall w : Walls)
	{
		w.Render(wallTexture);
	}

	//projectile rendering
	for (Projectile p : playerProjectiles)
	{
		p.Render(laserTexture);
	}
	for (Projectile e : enemyProjectiles)
	{
		e.Render(laserTexture);
	}

	//alien rendering  
	for (Alien a : Aliens)
	{
		a.Render(alienTexture);
	}
}

void Game::EndgameDraw()
{
	//Code
	if (newHighScore)
	{
		DrawText("NEW HIGHSCORE!", 600, 300, 60, YELLOW);

		// BELOW CODE IS FOR NAME INPUT RENDER
		DrawText("PLACE MOUSE OVER INPUT BOX!", 600, 400, 20, YELLOW);

		DrawRectangleRec(textBox, LIGHTGRAY);
		if (mouseOnText)
		{
			// HOVER CONFIRMIATION
			DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, RED);
		}
		else
		{
			DrawRectangleLines((int)textBox.x, (int)textBox.y, (int)textBox.width, (int)textBox.height, DARKGRAY);
		}

		//Draw the name being typed out
		DrawText(highscoreNameEntry.data(), (int)textBox.x + 5, (int)textBox.y + 8, 40, MAROON);

		//Draw the text explaining how many characters are used
		DrawText(TextFormat("INPUT CHARS: %i/%i", letterCount, 8), 600, 600, 20, YELLOW);

		if (mouseOnText)
		{
			if (letterCount < 9)
			{
				// Draw blinking underscore char
				if (((framesCounter / 20) % 2) == 0)
				{
					DrawText("_", (int)textBox.x + 8 + MeasureText(highscoreNameEntry.data(), 40), (int)textBox.y + 12, 40, MAROON);
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

		for (int i = 0; i < Leaderboard.size(); i++)
		{
			char* tempNameDisplay = Leaderboard[i].name.data();
			DrawText(tempNameDisplay, 50, 140 + (i * 40), 40, YELLOW);
			DrawText(TextFormat("%i", Leaderboard[i].score), 350, 140 + (i * 40), 40, YELLOW);
		}
	}
}

void Game::Shoot()
{
	if (IsKeyPressed(KEY_SPACE))
	{
		const Projectile newProjectile({ player.x_pos, static_cast<float>(GetScreenHeight()) }, playerProjectileSpeed);
		playerProjectiles.push_back(newProjectile);
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

bool Game::CheckNewHighScore()
{
	if (score > Leaderboard[4].score)
	{
		return true;
	}
	return false;
}

void Game::InsertNewHighScore(std::string& tName)
{
	PlayerData newData;
	newData.name = tName;
	newData.score = score;

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
	DrawTexture(texture.GetTexture(), (GetScreenWidth() / 2) - texture.WidthHalf() - 
		bgOffset, 0, WHITE);
}


