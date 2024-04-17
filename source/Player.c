#include <gba.h>

#include "Player.h"
#include "Projectile.h"
#include "SummonMain.h"

void updatePlayer(Player *player) {
	switch (player->summonState) {
		case SummonStateThirdSet:
//			ProjectileType prType = buildProjectileType(&player->incantations[0]);
			createProjectile(buildProjectileType(&player->incantations[0]));
			// Do summon
			player->summonState = SummonStateCoolDown;
			player->coolDown = 10;
			break;
		case SummonStateCoolDown:
			player->coolDown -= 1;
			if (player->coolDown <= 0) {
				player->summonState = SummonStateReady;
			}
			break;
		default:
			break;
	}
}

void playerIncantate(Player *player, Incantation incantation) {

	switch (player->summonState) {
		case SummonStateReady:
			player->incantations[0] = incantation;
			player->summonState = SummonStateFirstSet;
			break;
		case SummonStateFirstSet:
			player->incantations[1] = incantation;
			player->summonState = SummonStateSecondSet;
			break;
		case SummonStateSecondSet:
			player->incantations[2] = incantation;
			player->summonState = SummonStateThirdSet;
			break;
		default:
			break;
	}
}

void setPlayerVisualState(Player *player, VisualState state) {
	player->visualState = state;
	Mob *mob = &player->mob;

	switch (state) {
		case VisualStateStandingLeft:
			mob->xPos = 110;
			mob->yPos = 70;
			mob->tileIndex = 6;
			break;
		case VisualStateStandingRight:
			mob->xPos = 130;
			mob->yPos = 70;
			mob->tileIndex = 4;
			break;
		case VisualStateStandingUp:
			mob->xPos = 120;
			mob->yPos = 60;
			mob->tileIndex = 2;
			break;
		case VisualStateStandingDown:
			mob->xPos = 120;
			mob->yPos = 80;
			mob->tileIndex = 0;
			break;
		default:
			break;
	}
}

