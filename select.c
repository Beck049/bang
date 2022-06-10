#include "select.h"

void select_event_player(sGame *pGame, sSelectEvent *e) {
	printf("mx: %d, mn: %d\n", e->max_cnt, e->min_cnt);
	LIST_FOR_EACH(pNode, e->selections) {
		printf("'%s'\n", (char*)pNode->data);
	}
}

void select_event_bot(sGame *pGame, sSelectEvent *e) {
	
}