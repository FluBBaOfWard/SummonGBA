#ifndef PLAYER_HEADER
#define PLAYER_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "Mob.h"

/** The incantations */
typedef enum {
	IncantationNone = 0,
	IncantationFuh,
	IncantationRah,
	IncantationEnd
} Incantation;

/** Summoning state */
typedef enum {
	SummonStateReady = 0,
	SummonStateFirstSet,
	SummonStateSecondSet,
	SummonStateThirdSet,
	SummonStateCoolDown,
	SummonStateEnd
} SummonState;

/** Player struct */
typedef struct {
	Mob mob;
	SummonState summonState;
	VisualState visualState;
	Incantation incantations[4];
	int coolDown;
} Player;

/** This should run every frame */
void updatePlayer(Player *player);

/** Do one incantation */
void playerIncantate(Player *player, Incantation incantation);

/** Set direction of player */
void setPlayerVisualState(Player *player, VisualState state);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // PLAYER_HEADER
