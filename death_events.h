#pragma once

#include "event.h"
#include "game_play.h"

void death_event_default(sGame *pGame, sDeathEvent *e);
int getWinner( sGame * );