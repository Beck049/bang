#include "determine.h" 

void determine_event_default(sGame *pGame, sDetermineEvent *e) {
	char buf[BUFSIZ];
	printf("Determine Event:\n"
			"Draw 1 card:\n");
	i32 card_id = take_card(pGame, pGame->draw_pile, 0);
	print_card(buf, card_id);
	printf(" 1) %s\n", buf);
	give_card(pGame, pGame->discard_pile, card_id, false);
	e->determine_res = card_id;
}

void determine_event_lucky_duke(sGame *pGame, sDetermineEvent *e) {
	char buf[512];
	printf("Determine Event:\n"
			"Draw 2 cards:\n");
	i32 card_ids[2];
	char selections[2][1024];
	for(i32 i = 0; i < 2; ++i) {
		i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		card_ids[i] = card_id;
		print_card(buf, card_id);
		sprintf(selections[i], " %d) %s\n", i+1, buf);
	}

	sSelectEvent sl_e = select_event_with_arr(pGame, e->target_id, 1, 1, selections, 2, sizeof(*selections));
	i32 card_id = *(i32*)LIST_FRONT(sl_e.select_res);
	free_list(sl_e.selections);
	free_list(sl_e.select_res);

	for(i32 i = 0; i < 2; ++i) {
		give_card(pGame, pGame->discard_pile, card_ids[i], false);
	}
	
	e->determine_res = card_id;
}