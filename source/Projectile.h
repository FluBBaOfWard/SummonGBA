#ifndef PROJECTILE_HEADER
#define PROJECTILE_HEADER

#ifdef __cplusplus
extern "C" {
#endif

#include "Mob.h"
#include "Player.h"

/** The type of the projectile */
typedef enum {
	ProjectileTypeNone = 0,
	ProjectileTypeFire,
	ProjectileTypeWater,
	ProjectileTypeElectric,
	ProjectileTypeLight,
	ProjectileTypeEarth,
	ProjectileTypeWind,
	ProjectileTypeHeal,
	ProjectileTypeDeath,
	ProjectileTypeEnd
} ProjectileType;

/** The direction of the projectile */
typedef enum {
	ProjectileDirectionNone = 0,
	ProjectileDirectionLeft,
	ProjectileDirectionRight,
	ProjectileDirectionUp,
	ProjectileDirectionDown,
	ProjectileDirectionEnd
} ProjectileDirection;

/** Projectile struct */
typedef struct {
	Mob mob;
	ProjectileType type;
	ProjectileDirection direction;
} Projectile;

/** "Summons" a projectile from player in the direction the player is facing */
void summonProjectile(Player *player, Projectile *projectile, ProjectileType type);

/** Updates projectile, returns true if projectile died. */
bool updateProjectile(Projectile *projectile);

ProjectileType buildProjectileType(Incantation *incantations);

#ifdef __cplusplus
} // extern "C"
#endif

#endif // PROJECTILE_HEADER
