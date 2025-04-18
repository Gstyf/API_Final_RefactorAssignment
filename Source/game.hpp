#pragma once
#include <codeanalysis\warnings.h>
#pragma warning(push)
#pragma warning(disable:ALL_CODE_ANALYSIS_WARNINGS)
#include "raylib.h"
#pragma warning(pop)
#include "MyTexture.hpp"
#include "MyWindow.hpp"
#include "Player.hpp"
#include "Alien.hpp"
#include "Projectile.hpp"
#include "Wall.hpp"
#include <array>
#include <vector>
#include <string>
#include <stdexcept>

// TODO: Clean Game.h Too many objects are handled here that could have their own headers.
enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct PlayerData
{
	std::string name;
	int score;
};

class Background
{
	int bgOffset = 0;
public:
	void Update(int offset) noexcept;
	void Render(const MyTexture& tex) const noexcept;
};

struct Game
{
	explicit Game(State state) : gameState(state) {}

	// Window
	MyWindow myWindow;

	// Gamestate
	State gameState = {};

	MyTexture bgTexture{ "./Assets/Background.png" };
	MyTexture alienTexture{ "./Assets/Alien.png" };
	MyTexture wallTexture{ "./Assets/Wall.png" };
	MyTexture laserTexture{ "./Assets/Laser.png" };
	std::array<MyTexture, 3> shipTextures{
		MyTexture{"./Assets/Ship1.png"},
		MyTexture{"./Assets/Ship2.png"},
		MyTexture{"./Assets/Ship3.png"},
	};

	// Score
	int score;
	bool newHighScore = false;

	//Aliens shooting
	float shootTimer = 0;

	Player player{ static_cast<float>(GetScreenWidth() / 2) };
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;

	std::vector<PlayerData> Leaderboard = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

	Background background;

	//TEXTBOX ENTER
	std::string highscoreNameEntry = ""; 
	int letterCount = 0;

	Rectangle textBox = { 600, 500, 225, 50 };
	bool mouseOnText = false;

	int framesCounter = 0;

	void Start();
	void End();
	void Continue() noexcept;
	void Run();
	
	void Update();
	void GamePlayLogic();
	void EndScreenLogic();
	
	void Render();
	void EndgameDraw();
	void GamePlayDraw();

	void Shoot();
	void SpawnAliens();

	bool CheckNewHighScore();
	void InsertNewHighScore(std::string& name);
};