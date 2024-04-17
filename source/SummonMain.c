#include <gba.h>
#include <maxmod.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "SummonMain.h"
#include "io.h"
#include "Gfx.h"
#include "Player.h"
#include "Enemy.h"
#include "Projectile.h"

#include "Dungeon.h"
#include "Sprites.h"

#include "soundbank.h"
#include "soundbank_bin.h"

extern void simpleIrqHandler();

GameState gameState = GameStateStartScreen;

Player player;
Enemy enemies[32];
Projectile projectiles[32];

Mob *activeMobs[128];

int main() {

	irqInit();

	// Maxmod requires the vblank interrupt to reset sound DMA.
	// Link the VBlank interrupt to mmVBlank, and enable it. 
	irqSet( IRQ_VBLANK, mmVBlank );
	irqEnable(IRQ_VBLANK);

	// initialise maxmod with soundbank and 8 channels
    mmInitDefault( (mm_addr)soundbank_bin, 8 );
	mmSetVBlankHandler(simpleIrqHandler);

	mm_sound_effect ambulance = {
		{ SFX_AMBULANCE } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		0,		// panning
	};

	mm_sound_effect boom = {
		{ SFX_BOOM } ,			// id
		(int)(1.0f * (1<<10)),	// rate
		0,		// handle
		255,	// volume
		255,	// panning
	};

	// sound effect handle (for cancelling it later)
	mm_sfxhand amb = 0;

	SetMode(MODE_0 | BG0_ON | OBJ_ON);
	REG_BG0CNT = CHAR_BASE(0) | MAP_BASE(15);
	LZ77UnCompVram(DungeonTiles, (void *)VRAM);
	LZ77UnCompVram(DungeonMap, (void *)(VRAM + 0x7800));
	memcpy(BG_PALETTE, DungeonPal, 32);
	LZ77UnCompVram(SpritesTiles, (void *)(VRAM + 0x10000));
	memcpy(SPRITE_PALETTE, SpritesPal, 32);
	activeMobs[0] = &player.mob;
	setPlayerVisualState(&player, VisualStateStandingLeft);
	for ( int i = 0; i < 32; i++) {
		activeMobs[i+1] = &enemies[i].mob;
		activeMobs[i+33] = &projectiles[i].mob;
	}

	do {
		VBlankIntrWait();
//		randomNumber(0);

		scanKeys();
		switch (gameState) {
			case GameStatePlaying:
				runGameLoop();
				break;
			case GameStateNextLevel:
				nextLevel();
				gameState = GameStateGameOver;
				break;
			case GameStateStarting:
				initGame();
				break;
			case GameStateStartScreen:
				if ( keysDown() & KEY_A ) {
					gameState = GameStateStarting;
				}
				break;
			case GameStateGameOver:
				// Stop playing module
				mmStop( );

				player.mob.active = 0;
				gameState = GameStateStartScreen;
				break;
			default:
				break;
		}
		convertToSprites(activeMobs);


	} while( 1 );
}

void initGame() {
	// Start playing module
//	mmStart( MOD_SYS_UP_DATASTORM_UNFINISHED, MM_PLAY_LOOP );
	mmStart( MOD_DRIVINGNIGHTIME, MM_PLAY_LOOP );

	player.mob.active = 1;
	for ( int i = 0; i < 32; i++) {
		enemies[i].mob.active = 0;
		projectiles[i].mob.active = 0;
	}
	setLevel(0);
//	createEnemy(EnemyTypeDragon, VisualStateStandingDown);
//	createEnemy(EnemyTypeZombie, VisualStateStandingLeft);
//	createEnemy(EnemyTypeDeath, VisualStateStandingRight);
//	createEnemy(EnemyTypeSnail, VisualStateStandingUp);
	gameState = GameStatePlaying;
}

void runGameLoop() {
	int keys_pressed = keysDown();
	int keys_released = keysUp();

	if (runLevel()) {
		gameState = GameStateNextLevel;
	}
	if ( keys_pressed & KEY_LEFT ) {
		setPlayerVisualState(&player, VisualStateStandingLeft);
	}
	if ( keys_pressed & KEY_RIGHT ) {
		setPlayerVisualState(&player, VisualStateStandingRight);
	}
	if ( keys_pressed & KEY_UP ) {
		setPlayerVisualState(&player, VisualStateStandingUp);
	}
	if ( keys_pressed & KEY_DOWN ) {
		setPlayerVisualState(&player, VisualStateStandingDown);
	}

	// Play explosion sound effect out of right speaker if B button is pressed
	if ( keys_pressed & KEY_B ) {
		playerIncantate(&player, IncantationFuh);
//			mmEffectEx(&boom);
	}

	// Play looping ambulance sound effect out of left speaker if A button is pressed
	if ( keys_pressed & KEY_A ) {
		playerIncantate(&player, IncantationRah);
//			amb = mmEffectEx(&ambulance);
	}

	// stop ambulance sound when A button is released
//		if ( keys_released & KEY_A ) {
//			mmEffectCancel(amb);
//		}
	updatePlayer(&player);
	for (int i = 0; i<32; i++) {
		if (projectiles[i].mob.active == 1) {
			updateProjectile(&projectiles[i]);
		}
		if (enemies[i].mob.active == 1) {
			updateEnemy(&enemies[i]);
		}
	}
	handleCollision();
}

bool checkProjectileVsEnemy(ProjectileType projType, EnemyType enemyType) {
	if (projType == ProjectileTypeFire
		&& enemyType == EnemyTypeZombie) {
		return true;
	}
	if (projType == ProjectileTypeWater
		&& enemyType == EnemyTypeDragon) {
		return true;
	}
	if (projType == ProjectileTypeElectric
		&& enemyType == EnemyTypeSnail) {
		return true;
	}
	if (projType == ProjectileTypeLight
		&& enemyType == EnemyTypeDeath) {
		return true;
	}
	return false;
}

void handleCollision() {
	for (int i = 0; i<32; i++) {
		Mob *proj = &projectiles[i].mob;
		ProjectileType projType = projectiles[i].type;
		if (proj->active == 1) {
			for (int j = 0; j<32; j++) {
				Mob *enemy = &enemies[j].mob;
				if (enemy->active == 1
					&& abs(proj->xPos - enemy->xPos) < 16
					&& abs(proj->yPos - enemy->yPos) < 16) {
					if ( checkProjectileVsEnemy(projType, enemies[j].type)) {
						enemy->active = 0;
					}
					proj->active = 0;
				}
			}
		}
	}
	// Check for player damage
	for (int i = 0; i<32; i++) {
		Mob *enemy = &enemies[i].mob;
		if (enemy->active == 1
			&& (enemy->xPos>>4) == 7
			&& (enemy->yPos>>4) == 4) {
			gameState = GameStateGameOver;
		}
	}
}

void createProjectile(ProjectileType type) {
	for (int i = 0; i<32; i++) {
		if (projectiles[i].mob.active == 0) {
			summonProjectile(&player, &projectiles[i], type);
			return;
		}
	}
}

void createEnemy(EnemyType type, VisualState direction) {
	for (int i = 0; i<32; i++) {
		if (enemies[i].mob.active == 0) {
			summonEnemy(&enemies[i], type, direction);
			return;
		}
	}
}
