#pragma once

#include "basic.h"
#include "player.h"
#include "init_card.h"
#include "init_game.h"
#include "character.h"
#include "shuffle.h"
#include "draw.h"

// set up player info
void players_setup(sGame *pGame) {
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