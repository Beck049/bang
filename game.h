#pragma once

#include "basic.h"
#include "list.h"
#include "player.h"
#include "shuffle.h"
#include "init_card.h"
#include "init_character.h"

typedef struct _sGame {
	bool is_end;
	sList *draw_pile;
	sList *discard_pile;
	sList *role_pile;
	sList *character_pile;
	sList *live_players;
	sListNode *cur_player;
	sPlayer players[MAX_PLAYERS];
	i32 total_players;
} sGame;

sGame *new_game();

void free_game(sGame *pGame);