#ifndef ENEMY_HEADER
#define ENEMY_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "Mob.h"

/** The type of the enemy */
typedef enum {
	EnemyTypeNone = 0,
	EnemyTypeDragon,
	EnemyTypeSnail,
	EnemyTypeZombie,
	EnemyTypeDeath,
	EnemyTypeEnd
} EnemyType;

typedef enum {
	ActionReady = 0,
	ActionFirstSet,
	ActionSecondSet,
	ActionThirdSet,
	ActionEnd
} ActionState;

/** Enemy struct */
typedef struct {
	Mob mob;
	EnemyType type;
	ActionState actionState;
	VisualState visualState;
	int speedCounter;
} Enemy;

void summonEnemy(Enemy *enemy, EnemyType type, VisualState direction);

bool updateEnemy(Enemy *enemy);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // ENEMY_HEADER
