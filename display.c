#include "display.h"

void display_pile(sList *pList) {
	static const char SUIT[] = "SHDC";
	i32 row = 1;
	LIST_FOR_EACH(pNode, pList) {
		i32 card_id = *(i32*)pNode->data;
		sCard *card = cards+card_id;
		printf("%2d) %c%-2d %s: %s\n", row, SUIT[card->suit], card->num, card->name, card->description);
		++row;
	}
}

void display_game(sGame *pGame, i32 target_id) {
	printf( "-----------------BANG-----------------\n"
 			" Draw Pile (%ld)   /  Discard Pile (%ld)\n"
			"--------------------------------------\n", pGame->draw_pile->size, pGame->discard_pile->size);
	sList *live_player = pGame->live_players;

	sListNode *player_node = get_player(pGame, target_id);
	if(player_node == NULL) {
		player_node = LIST_FRONT(live_player);
	}

	i32 size = live_player->size;
	for(i32 i = 0; i < size; ++i) {
		i32 player_id = *(i32*)player_node->data;
		sList *player_cards = pGame->players[player_id].cards;
		sList *player_desk = pGame->players[player_id].desk;
		char appellation[16];
		if(player_id == target_id) {
			sprintf(appellation, "Your");
		}
		else {
			sprintf(appellation, "Player%d's", player_id);
		}
		printf("%s Cards (%ld):\n", appellation, player_cards->size);
		display_pile(player_cards);
		printf("%s Desk (%ld):\n", appellation, player_desk->size);
		display_pile(player_desk);
	}
}