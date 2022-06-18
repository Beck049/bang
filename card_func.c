#include "card_func.h"


void card_miss(sGame *pGame, i32 player_id) {

}

void card_bang(sGame *pGame, i32 player_id) {

}

void card_saldon(sGame *pGame, i32 player_id) {
	i32 arr[10] = {0};
	i32 num = pGame->live_players->size;
	sListNode *cur_p = get_player(pGame, player_id);
	for(i32 i = 0; i < num; ++i) {
		arr[i] = take_card(pGame, pGame->draw_pile, 0);
	}
	char cards_opt[num][32];
	for(int i = 0; i < num; ++i ) {

		sprintf(cards_opt[i], "pick a card : %d", i);
	}
	for(i32 i = 0; i < num; ++i ) {
		// draw
		// print all card
		sSelectEvent event = select_event_with_arr(pGame, cur_p, 1, 1, cards_opt, num-i, 32);
		i32 take_id = *(i32*)LIST_FRONT(event.select_res);
		i32 card_id = arr[take_id];
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);

		strcpy(cards_opt[take_id], cards_opt[num-i-1]);

		cur_p = cur_p->next;
	}
}

void card_stagecoach(sGame *pGame, i32 player_id) {
	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_id;
	for(i32 i = 0; i < 2; ++i) {
		card_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);
	}
}

void card_wells_fargo(sGame *pGame, i32 player_id) {
	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_id;
	for(i32 i = 0; i < 3; ++i) {
		card_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);
	}
}

void card_beer(sGame *pGame, i32 player_id) {
	if( (i32)pGame->live_players->size >= 2  ) {
		i32 id = (i32)pGame->cur_player->data;
		pGame->players[id].hp += 1;
	}
}

void card_cat_balou(sGame *pGame, i32 player_id) {
	
}

void (*card_funcs[80])(sGame *pGame, i32 player_id) = {
	&card_miss,
	&card_bang,
};