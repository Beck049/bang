#include "game.h"

sGame *new_game() {
	sGame *pGame = calloc(1, sizeof(sGame));
	pGame->draw_pile = new_list();
	pGame->discard_pile = new_list();
	pGame->role_pile = new_list();
	pGame->character_pile = new_list();
	pGame->live_players = new_list();
	for(i32 i = 0; i < MAX_PLAYERS; ++i) {
		pGame->players[i].cards = new_list();
		pGame->players[i].desk  = new_list();
	}
	return pGame;
}

void free_game(sGame *pGame) {
	free_list(pGame->draw_pile);
	free_list(pGame->discard_pile);
	free_list(pGame->role_pile);
	free_list(pGame->character_pile);
	free_list(pGame->live_players);
	for(i32 i = 0; i < MAX_PLAYERS; ++i) {
		free_list(pGame->players[i].cards);
		free_list(pGame->players[i].desk);
	}
	free(pGame);
}
