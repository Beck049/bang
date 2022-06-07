#pragma once

#include "basic.h"

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


// init game
void game_init(sGame *pGame, i32 num_players) {
	pGame->total_players = num_players;
	draw_pile_init(pGame);
	role_pile_init(pGame);
	character_pile_init(pGame);
	live_players_init(pGame);
	players_setup(pGame);
	pGame->cur_player = LIST_BEGIN(pGame->live_players);
}

// init role pile and shuffle
void role_pile_init(sGame *pGame) {
	for(i32 i = 0; i < ROLE_SIZ; ++i) {  // loop role
		// push role into list
		for(i32 j = 0; j < ROLE_DIVIDED[pGame->total_players][i]; ++j) {
			eRole *role = malloc(sizeof(eRole));
			*role = i;
			list_push_back(pGame->role_pile, new_node(role));
		}
	}
	shuffle(pGame->role_pile);
}

// init character pile and shuffle
void character_pile_init(sGame *pGame) {
	list_init(pGame->character_pile, character_num);
	shuffle(pGame->character_pile);
}

// init live players
void live_players_init(sGame *pGame) {
	list_init(pGame->live_players, pGame->total_players);
	shuffle(pGame->live_players);
}