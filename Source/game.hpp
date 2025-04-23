#pragma once
#include "codeanalysis/warnings.h"
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

enum struct State
{
	STARTSCREEN,
	GAMEPLAY,
	ENDSCREEN
};

struct PlayerData
{
	PlayerData(const std::string& n, int s) : name(n), score(s) { }
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

class Game
{
public:
	explicit Game(State state) : gameState(state) {}

	// Window
	MyWindow myWindow{ _WINDOW_TITLE };

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
	int score = 0;
	bool newHighScore = false;

	//Aliens shooting
	float shootTimer = 0;

	Player player{{ _screenHalfWidthF, _screenHeightF - playerFloorOffsetY }};
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;

	Background background;

	std::vector<PlayerData> Leaderboard = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

	//TEXTBOX ENTER
	std::string highscoreNameEntry = ""; 
	int letterCount = 0;

	Rectangle textBox = { 600, 500, 225, 50 };
	bool mouseOnText = false;

	int framesCounter = 0;

	void Start();
	void End() noexcept;
	void Continue() noexcept;
	void Run();
	
	void Update();
	void GameplayUpdate();
	void CheckIfGameOver() noexcept;
	void UpdateProjectiles() noexcept;
	void ResolveProjectileCollisions() noexcept;
	void Shoot();
	void AlienShoot();
	void SpawnAliens();
	void RemoveDeadEntities();
	
	void EndscreenUpdate();
	void HandleKeyboardInput();
	
	void Render();
	void EndgameDraw();
	void HandleMouseOverNameInput() noexcept;
	void GamePlayDraw() const noexcept;


	bool CheckNewHighScore() noexcept;
	void InsertNewHighScore(const std::string& name);
};
