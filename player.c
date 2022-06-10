#include "player.h"

sPlayer *new_player() {
	sPlayer *pPlayer = calloc(1, sizeof(sPlayer));
	pPlayer->cards = new_list();
	pPlayer->desk = new_list();
	return pPlayer;
}

void free_player(sPlayer *pPlayer) {
	free_list(pPlayer->cards);
	free_list(pPlayer->desk);
	free(pPlayer);
}