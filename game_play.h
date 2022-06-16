#pragma once

#include "basic.h"
#include "player.h"
#include "init_card.h"
#include "init_game.h"
#include "init_character.h"
#include "shuffle.h"
#include "discard.h"
// #include "draw.h"

sListNode *get_player(sGame *pGame, i32 id);

sListNode *get_prev_player(sGame *pGame, sListNode *pPlayerNode);

sListNode *get_next_player(sGame *pGame, sListNode *pPlayerNode);

sList *card_filter(sList *cards, bool (*filter)(i32 card_id));

i32 prep_phase(sGame *pGame);

void draw_phase(sGame *pGame);

void play_phase(sGame *pGame);

void discard_phase(sGame *pGame);

void turn_phase(sGame *pGame);

void game_end(sGame *pGame);

void play_game(i32 num_players);