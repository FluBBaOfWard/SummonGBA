#include <gba.h>

#include "Levels.h"
#include "Enemy.h"
#include "SummonMain.h"

/** Spawn struct */
typedef struct {
	int waitTime;
	EnemyType type;
	VisualState direction;
} Spawn;

int currentLevel = 0;
int spawnIndex = 0;
int waitCounter = 0;

Spawn level0[] = {
	{2*60, EnemyTypeZombie, VisualStateStandingLeft},
	{2*60, EnemyTypeZombie, VisualStateStandingLeft},
	{2*60, EnemyTypeZombie, VisualStateStandingDown},
	{2*60, EnemyTypeZombie, VisualStateStandingDown},
	{2*60, EnemyTypeZombie, VisualStateStandingRight},
	{2*60, EnemyTypeZombie, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},

	{1*60, EnemyTypeZombie, VisualStateStandingLeft},
	{1*60, EnemyTypeZombie, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingDown},
	{1*60, EnemyTypeZombie, VisualStateStandingLeft},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*60, EnemyTypeZombie, VisualStateStandingDown},
	{1*60, EnemyTypeZombie, VisualStateStandingRight},

	{3*60, EnemyTypeDragon, VisualStateStandingLeft},
	{3*60, EnemyTypeDragon, VisualStateStandingLeft},
	{2*60, EnemyTypeDragon, VisualStateStandingDown},
	{2*60, EnemyTypeDragon, VisualStateStandingRight},
	{2*60, EnemyTypeDragon, VisualStateStandingUp},

	{3*60, EnemyTypeDragon, VisualStateStandingRight},
	{2*60, EnemyTypeDragon, VisualStateStandingRight},
	{2*60, EnemyTypeZombie, VisualStateStandingDown},
	{2*60, EnemyTypeDragon, VisualStateStandingLeft},
	{2*60, EnemyTypeZombie, VisualStateStandingUp},

	{3*60, EnemyTypeDragon, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingDown},
	{1*60, EnemyTypeDragon, VisualStateStandingLeft},
	{1*60, EnemyTypeDragon, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*60, EnemyTypeZombie, VisualStateStandingDown},

	{3*60, EnemyTypeSnail, VisualStateStandingRight},
	{3*60, EnemyTypeSnail, VisualStateStandingRight},
	{2*60, EnemyTypeZombie, VisualStateStandingLeft},
	{2*60, EnemyTypeSnail, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*60, EnemyTypeSnail, VisualStateStandingDown},

	{3*60, EnemyTypeZombie, VisualStateStandingUp},
	{3*60, EnemyTypeSnail, VisualStateStandingRight},
	{2*60, EnemyTypeDragon, VisualStateStandingLeft},
	{2*60, EnemyTypeSnail, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*60, EnemyTypeSnail, VisualStateStandingDown},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},

	{3*60, EnemyTypeDeath, VisualStateStandingRight},
	{3*60, EnemyTypeDeath, VisualStateStandingRight},
	{2*60, EnemyTypeDragon, VisualStateStandingLeft},
	{2*60, EnemyTypeSnail, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*60, EnemyTypeSnail, VisualStateStandingDown},
	{1*60, EnemyTypeDragon, VisualStateStandingUp},

	{1*60, EnemyTypeDeath, VisualStateStandingRight},
	{1*60, EnemyTypeDeath, VisualStateStandingRight},
	{1*60, EnemyTypeDragon, VisualStateStandingLeft},
	{1*60, EnemyTypeSnail, VisualStateStandingRight},
	{1*60, EnemyTypeZombie, VisualStateStandingUp},
	{1*30, EnemyTypeZombie, VisualStateStandingDown},
	{1*30, EnemyTypeZombie, VisualStateStandingUp},
	{1*30, EnemyTypeZombie, VisualStateStandingRight},
	{1*30, EnemyTypeZombie, VisualStateStandingLeft},
	{1*30, EnemyTypeZombie, VisualStateStandingDown},
	{1*30, EnemyTypeZombie, VisualStateStandingUp},
	{1*30, EnemyTypeZombie, VisualStateStandingRight},
	{1*30, EnemyTypeZombie, VisualStateStandingLeft},

	{5*60, EnemyTypeEnd, VisualStateStandingRight},
};

void setLevel(int level) {
	currentLevel = level;
	spawnIndex = 0;
	waitCounter = level0[spawnIndex].waitTime;
}

bool runLevel() {
	waitCounter -= 1;
	if (waitCounter <= 0) {
		Spawn spawn = level0[spawnIndex];
		EnemyType enemy = spawn.type;
		if (enemy == EnemyTypeEnd) {
			return true;
		}
			createEnemy(spawn.type, spawn.direction);
			spawnIndex += 1;
			waitCounter = level0[spawnIndex].waitTime;
	}
	return false;
}

bool nextLevel() {
	setLevel(currentLevel + 1);
	return false;
}
