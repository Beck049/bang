#pragma once

#include "basic.h"
#include "player.h"
#include "card.h"
#include "character.h"
#include "shuffle.h"
#include "draw.h"

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

// player get N cards form draw pile
i32 draw_card(sGame *pGame, i32 player_id, size_t cnt) {
	if(pGame->draw_pile->size < cnt) return -1;  // no card to draw
	for(size_t i = 0; i < cnt; ++i) {
		i32 *dup_id = malloc(sizeof(i32));
		*dup_id = *(i32*)draw(pGame->draw_pile);
		list_push_back(pGame->players[player_id].cards, new_node(dup_id));
	}
	return 0;
}

// set up draw pile and shuffle
void draw_pile_init(sGame *pGame) {
	list_init(pGame->draw_pile, card_num);
	shuffle(pGame->draw_pile);
}

// set up role pile and shuffle
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

void character_pile_init(sGame *pGame) {
	list_init(pGame->character_pile, character_num);
	shuffle(pGame->character_pile);
}

void players_setup(sGame *pGame) {
	list_init(pGame->live_players, pGame->total_players);
	shuffle(pGame->live_players);

	role_pile_init(pGame);
	character_pile_init(pGame);

	for(i32 i = 0; i < pGame->total_players; ++i) {
		pGame->players[i].id = i;
		pGame->players[i].attack_range = 1;
		pGame->players[i].role = *(eRole*)draw(pGame->role_pile);
		pGame->players[i].character = *(i32*)draw(pGame->character_pile);

		// TODO setup hp
		// pGame->players[i].hp = ;
		
		draw_card(pGame, i, pGame->players[i].hp);
	}
}

void game_init(sGame *pGame, i32 num_players) {
	pGame->total_players = num_players;
	draw_pile_init(pGame);
	players_setup(pGame);
}

void prep_phase(sGame *pGame);  // determine bomb jail
// [name=Beck]

void draw_phase(sGame *pGame);  // draw two cards

void play_phase(sGame *pGame);  // play any number of cards

void discard_phase(sGame *pGame);  // discard excess cards

void turn_phase(sGame *pGame) {
	// print_game(pGame);
	// prep_phase(pGame);
	// draw_phase(pGame);
	// play_phase(pGame);
	// discard_phase(pGame);
}

void game_end(sGame *pGame) {

}

void game_cleanup(sGame *pGame) {
	list_clear(pGame->draw_pile);
	list_clear(pGame->discard_pile);
	list_clear(pGame->role_pile);
	list_clear(pGame->character_pile);
	list_clear(pGame->live_players);
}

sGame *new_game() {
	sGame *pGame = calloc(1, sizeof(sGame));
	pGame->draw_pile = new_list();
	pGame->discard_pile = new_list();
	pGame->role_pile = new_list();
	pGame->character_pile = new_list();
	pGame->live_players = new_list();
	return pGame;
}

void free_game(sGame *pGame) {
	free_list(pGame->draw_pile);
	free_list(pGame->discard_pile);
	free_list(pGame->role_pile);
	free_list(pGame->character_pile);
	free_list(pGame->live_players);
	free(pGame);
}

void play_game(i32 num_players) {
	sGame *pGame = new_game();  // malloc
	game_init(pGame, num_players);  // set up pGame
	// while(!pGame->is_end) {
		// turn_phase(pGame);  // process pGame
	// }
	game_end(pGame);
	free_game(pGame);
}