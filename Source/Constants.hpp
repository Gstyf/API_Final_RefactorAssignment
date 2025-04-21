#include <string_view>

//-- CONSTANTS HEADER --//
//Window Constants
constexpr int _WINDOW_HEIGHT = 900;
constexpr int _WINDOW_WIDTH = 1800;
constexpr std::string_view _WINDOW_TITLE = "SPACE INVADER!!";

//Wall Constants
constexpr float wallCount = 5;

//Projectile Constants
constexpr int playerProjectileSpeed = 15;
constexpr int enemyProjectileSpeed = -15;

//Alien Spawning Constants
constexpr int formationWidth = 8;
constexpr int formationHeight = 5;
constexpr int alienSpacing = 90;
constexpr float formationX = 100;
constexpr float formationY = 50;