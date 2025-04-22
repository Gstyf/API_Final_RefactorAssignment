#pragma once
#include <string_view>

//-- CONSTANTS HEADER --//
//Window Constants
constexpr int _WINDOW_HEIGHT = 900;
constexpr int _WINDOW_WIDTH = 1800;
constexpr std::string_view _WINDOW_TITLE = "SPACE INVADER!!";
constexpr float _screenHeightF = _WINDOW_HEIGHT;
constexpr float _screenHalfHeightF = _screenHeightF / 2;
constexpr float _screenWidthF = _WINDOW_WIDTH;
constexpr float _screenHalfWidthF = _screenWidthF / 2;

//Wall Constants
constexpr int wallCount = 5;
constexpr int wallHealthOffsetX = 21;
constexpr float wallOffsetY = 200;

//Projectile Constants
constexpr int playerProjectileSpeed = 15;
constexpr int enemyProjectileSpeed = -15;
constexpr int projectileSpawnOffset = 50;

//Alien Constants
constexpr int alienForwardMovement = 50;
constexpr int formationWidth = 8;
constexpr int formationHeight = 5;
constexpr int alienSpacing = 90;
constexpr float formationX = 100;
constexpr float formationY = 50;

//Player Constants
constexpr int playerFloorOffsetY = 50;