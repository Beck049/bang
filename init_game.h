#pragma once

#include "game.h"
#include "event.h"

// init draw pile and shuffle
void draw_pile_init(sGame *pGame);

// init role pile and shuffle
void role_pile_init(sGame *pGame);
// init character pile and shuffle
void character_pile_init(sGame *pGame);

// init live players
void live_players_init(sGame *pGame);

// set up player info
void players_setup(sGame *pGame);

// init game
void game_init(sGame *pGame, i32 num_players);

void game_cleanup(sGame *pGame);