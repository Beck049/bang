#include "bang.h"
#include "display.h"

void bang_event_default(sGame *pGame, sBangEvent *e) {
	// display_bang(pGame, 0, *e);  // display to player 0
	sDodgeEvent dg = dodge_event(pGame, e->target_id, 1);
	// if dodge success -> bang failed
	e->bang_res = !dg.dodge_res;
}

void bang_event_slab_the_killer(sGame *pGame, sBangEvent *e) {
	// display_bang(pGame, 0, *e);  // display to player 0
	sDodgeEvent dg = dodge_event(pGame, e->target_id, 2);
	// if dodge success -> bang failed
	e->bang_res = !dg.dodge_res;
}