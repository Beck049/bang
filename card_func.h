#pragma once

#include "game_play.h"
#include "draw.h"
#include "event.h"

void card_saldon(sGame *pGame, i32 player_id);
void card_stagecoach(sGame *pGame, i32 player_id);
void card_wells_fargo(sGame *pGame, i32 player_id);
void card_beer(sGame *pGame, i32 player_id);

extern void (*card_funcs[80])(sGame *pGame, i32 player_id);