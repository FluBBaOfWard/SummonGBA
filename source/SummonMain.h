#ifndef SUMMON_HEADER
#define SUMMON_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "Projectile.h"
#include "Enemy.h"

/** Game state */
typedef enum {
	GameStateNone = 0,
	GameStateStartScreen,
	GameStateDemo,
	GameStateStarting,
	GameStatePlaying,
	GameStateNextLevel,
	GameStateGameOver,
	GameStateRestart
} GameState;


/** Create a projectile from the player */
void createProjectile(ProjectileType type);

/** Create an enemy in the specified direction */
void createEnemy(EnemyType type, VisualState direction);

void handleCollision(void);

void initGame(void);

void runGameLoop(void);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // SUMMON_HEADER
