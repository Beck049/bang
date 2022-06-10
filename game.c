#include "game.h"

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
