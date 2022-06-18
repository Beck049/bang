#pragma once

#include "game.h"
#include "event.h"
#include "draw.h"

// init draw pile and shuffle
void init_draw_pile(sGame *pGame);

// init role pile and shuffle
void init_role_pile(sGame *pGame);

// init character pile and shuffle
void init_character_pile(sGame *pGame);

// init live players
void live_players_init(sGame *pGame);

// set up player info
void setup_players(sGame *pGame);

// init game
void init_game(sGame *pGame, i32 num_players);

void cleanup_game(sGame *pGame);
