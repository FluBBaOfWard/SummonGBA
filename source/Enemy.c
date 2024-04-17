#include <gba.h>

#include "Enemy.h"
#include "Player.h"

void summonEnemy(Enemy *enemy, EnemyType type, VisualState direction) {
	Mob *enMob = &enemy->mob;
	switch (direction) {
		case VisualStateStandingLeft:
			enemy->visualState = VisualStateStandingLeft;
			enMob->xPos = 224;
			enMob->yPos = 70;
			break;
		case VisualStateStandingRight:
			enemy->visualState = VisualStateStandingRight;
			enMob->xPos = 0;
			enMob->yPos = 70;
			break;
		case VisualStateStandingUp:
			enemy->visualState = VisualStateStandingUp;
			enMob->xPos = 120;
			enMob->yPos = 144;
			break;
		case VisualStateStandingDown:
			enemy->visualState = VisualStateStandingDown;
			enMob->xPos = 120;
			enMob->yPos = 0;
			break;
		default:
			break;
	}
	enemy->type = type;
	switch (type) {
		case EnemyTypeDragon:
			enMob->tileIndex = 16;
			break;
		case EnemyTypeSnail:
			enMob->tileIndex = 32;
			break;
		case EnemyTypeZombie:
			enMob->tileIndex = 48;
			break;
		case EnemyTypeDeath:
			enMob->tileIndex = 64;
			break;
		default:
			break;
	}
	enMob->active = 1;
}

bool updateEnemy(Enemy *enemy) {
	int counter = enemy->speedCounter;
	counter -= 1;
	bool died = false;
	if ( counter <= 0) {
		counter = 9;
		Mob *mob = &enemy->mob;
		
		switch (enemy->visualState) {
			case VisualStateStandingLeft:
				mob->xPos -= 1;
				break;
			case VisualStateStandingRight:
				mob->xPos += 1;
				break;
			case VisualStateStandingUp:
				mob->yPos -= 1;
				break;
			case VisualStateStandingDown:
				mob->yPos += 1;
				break;
			default:
				break;
		}
		if ( mob->xPos <= -16 || mob->xPos >= 256 || mob->yPos <= -16  || mob->yPos >= 240) {
			enemy->visualState = VisualStateEnd;
			mob->active = 0;
			died = true;
		}
	}
	enemy->speedCounter = counter;
	return died;
}
