#ifndef MOB_HEADER
#define MOB_HEADER

#ifdef __cplusplus
	extern "C" {
#endif

typedef enum {
	VisualStateDefault = 0,
	VisualStateStandingLeft = 0,
	VisualStateStandingRight = 1,
	VisualStateStandingUp = 2,
	VisualStateStandingDown = 3,
	VisualStateEnd
} VisualState;


/** Mob struct */
typedef struct {
	int xPos;
	int yPos;
	char tileIndex;
	char active;
	char padding[2];
} Mob;


#ifdef __cplusplus
	} // extern "C"
#endif

#endif // MOB_HEADER
