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
