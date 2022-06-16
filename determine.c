#include "determine.h" 

void determine_event_default(sGame *pGame, sDetermineEvent *e) {
	char buf[BUFSIZ];
	printf("Determine Event:\n"
			"Draw 1 card:\n");
	e->determine_res = take_card(pGame, pGame->draw_pile, 0);
	print_card(buf, e->determine_res);
	printf(" 1) %s\n", buf);
}
