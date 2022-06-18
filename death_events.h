#pragma once

#include "event.h"
#include "game_play.h"

void death_event_default(sGame *, sDeathEvent *);
eRole getWinner( sGame * );
void death_event_Vulture_Sam(sGame *, sDeathEvent *);