#pragma once

#include "basic.h"
#include "player.h"
#include "init_card.h"
#include "init_game.h"
#include "init_character.h"
#include "shuffle.h"
// #include "draw.h"

void prep_phase(sGame *pGame);

void draw_phase(sGame *pGame);

void play_phase(sGame *pGame);

void discard_phase(sGame *pGame);

void turn_phase(sGame *pGame);

void game_end(sGame *pGame);

void play_game(i32 num_players);