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


struct Star
{
	Vector2 initPosition = { 0, 0 };
	Vector2 position = { 0, 0 };
	Color color = GRAY;
	float size = 0;
	void Update(float starOffset);
	void Render();
};

//TODO: Remove background class, just have it be a texture.
struct Background
{
	std::vector<Star> Stars;

	void Initialize(int starAmount);
	void Update(float offset);
	void Render();
};

struct Game
{
	explicit Game(State state) : gameState(state) {}

	// Window
	MyWindow myWindow;

	// Gamestate
	State gameState = {};

	//TODO: Move resources to their respective class?
	MyTexture alienTexture{ "./Assets/Alien.png" };
	MyTexture wallTexture{ "./Assets/Wall.png" };
	MyTexture laserTexture{ "./Assets/Laser.png" };
	std::array<MyTexture,3> shipTextures{
		MyTexture{"./Assets/Ship1.png"},
		MyTexture{"./Assets/Ship2.png"},
		MyTexture{"./Assets/Ship3.png"},
	};

	// Score
	int score;

	//Aliens shooting
	float shootTimer = 0;

	bool newHighScore = false;

	Player player{static_cast<float>(GetScreenWidth() / 2)};
	std::vector<Wall> Walls;
	std::vector<Alien> Aliens;

	std::vector<Projectile> playerProjectiles;
	std::vector<Projectile> enemyProjectiles;

	std::vector<PlayerData> Leaderboard = { {"Player 1", 500}, {"Player 2", 400}, {"Player 3", 300}, {"Player 4", 200}, {"Player 5", 100} };

	Background background;
	float offset;

	//TEXTBOX ENTER
	char name[9 + 1] = "\0";      //One extra space required for null terminator char '\0'
	int letterCount = 0;

	Rectangle textBox = { 600, 500, 225, 50 };
	bool mouseOnText = false;

	int framesCounter = 0;

	void Start();
	void End();

	void Continue();

	void Run();
	void Update();
	void Render();

	void Shoot();
	void SpawnAliens();

	bool CheckNewHighScore();
	void InsertNewHighScore(std::string& name);
};