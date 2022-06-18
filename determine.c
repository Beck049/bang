#include "determine.h" 

void determine_event_default(sGame *pGame, sDetermineEvent *e) {
	char buf[BUFSIZ];
	display_determine(pGame, e->target_id, 1);
	i32 card_id = take_card(pGame, pGame->draw_pile, 0);
	print_card(buf, card_id);
	printf(" 1) %s", buf);
	give_card(pGame, pGame->discard_pile, card_id, false);
	e->determine_res = card_id;
}

void determine_event_lucky_duke(sGame *pGame, sDetermineEvent *e) {
	char buf[512];
	display_determine(pGame, e->target_id, 2);
	i32 cards_id[2];
	char selections[2][1024];
	for(i32 i = 0; i < 2; ++i) {
		i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		cards_id[i] = card_id;
		print_card(buf, card_id);
		sprintf(selections[i], " %d) %s", i+1, buf);
	}

	sSelectEvent sl_e = select_event_with_arr(pGame, e->target_id, 1, 1, selections, 2, sizeof(*selections));
	i32 card_id = *(i32*)LIST_FRONT(sl_e.select_res);
	free_list(sl_e.selections);
	free_list(sl_e.select_res);

	for(i32 i = 0; i < 2; ++i) {
		give_card(pGame, pGame->discard_pile, cards_id[i], false);
	}
	
	e->determine_res = card_id;
}