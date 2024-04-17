#include <gba.h>

#include "Projectile.h"
#include "Player.h"

void summonProjectile(Player *player, Projectile *projectile, ProjectileType type) {
	Mob *plMob = &player->mob;
	Mob *prMob = &projectile->mob;
	switch (player->visualState) {
		case VisualStateStandingLeft:
			projectile->direction = ProjectileDirectionLeft;
			prMob->xPos = plMob->xPos - 16;
			prMob->yPos = plMob->yPos;
			break;
		case VisualStateStandingRight:
			projectile->direction = ProjectileDirectionRight;
			prMob->xPos = plMob->xPos + 16;
			prMob->yPos = plMob->yPos;
			break;
		case VisualStateStandingUp:
			projectile->direction = ProjectileDirectionUp;
			prMob->xPos = plMob->xPos;
			prMob->yPos = plMob->yPos - 16;
			break;
		case VisualStateStandingDown:
			projectile->direction = ProjectileDirectionDown;
			prMob->xPos = plMob->xPos;
			prMob->yPos = plMob->yPos + 16;
			break;
		default:
			break;
	}
	projectile->type = type;
	switch (type) {
		case ProjectileTypeFire:
			prMob->tileIndex = 240;
			break;
		case ProjectileTypeWater:
			prMob->tileIndex = 241;
			break;
		case ProjectileTypeElectric:
			prMob->tileIndex = 242;
			break;
		case ProjectileTypeLight:
			prMob->tileIndex = 243;
			break;
		case ProjectileTypeEarth:
			prMob->tileIndex = 244;
			break;
		case ProjectileTypeWind:
			prMob->tileIndex = 245;
			break;
		case ProjectileTypeHeal:
			prMob->tileIndex = 246;
			break;
		case ProjectileTypeDeath:
			prMob->tileIndex = 247;
			break;
		default:
			break;
	}
	prMob->active = 1;
}

bool updateProjectile(Projectile *projectile) {
	Mob *mob = &projectile->mob;

	switch (projectile->direction) {
		case ProjectileDirectionLeft:
			mob->xPos -= 2;
			break;
		case ProjectileDirectionRight:
			mob->xPos += 2;
			break;
		case ProjectileDirectionUp:
			mob->yPos -= 2;
			break;
		case ProjectileDirectionDown:
			mob->yPos += 2;
			break;
		default:
			return false;
	}
	if ( mob->xPos <= -16 || mob->xPos >= 256 || mob->yPos <= -16  || mob->yPos >= 240) {
		projectile->direction = ProjectileDirectionEnd;
		mob->active = 0;
		return true;
	}
	return false;
}

ProjectileType buildProjectileType(Incantation *incantations) {
	if (incantations[0] == IncantationFuh
			 && incantations[1] == IncantationFuh
			 && incantations[2] == IncantationFuh) {
		return ProjectileTypeFire;
	}
	else if (incantations[0] == IncantationFuh
			 && incantations[1] == IncantationFuh
			 && incantations[2] == IncantationRah) {
		return ProjectileTypeWater;
	}
	else if (incantations[0] == IncantationFuh
			 && incantations[1] == IncantationRah
			 && incantations[2] == IncantationFuh) {
		return ProjectileTypeElectric;
	}
	else if (incantations[0] == IncantationFuh
			 && incantations[1] == IncantationRah
			 && incantations[2] == IncantationRah) {
		return ProjectileTypeLight;
	}
	else if (incantations[0] == IncantationRah
			 && incantations[1] == IncantationFuh
			 && incantations[2] == IncantationFuh) {
		return ProjectileTypeEarth;
	}
	else if (incantations[0] == IncantationRah
			 && incantations[1] == IncantationFuh
			 && incantations[2] == IncantationRah) {
		return ProjectileTypeWind;
	}
	else if (incantations[0] == IncantationRah
			 && incantations[1] == IncantationRah
			 && incantations[2] == IncantationFuh) {
		return ProjectileTypeHeal;
	}
	else if (incantations[0] == IncantationRah
			 && incantations[1] == IncantationRah
			 && incantations[2] == IncantationRah) {
		return ProjectileTypeDeath;
	}
	return ProjectileTypeNone;
}
