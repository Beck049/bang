#pragma once

#include "basic.h"
#include "list.h"


void *draw(sList *pList) {
    void *ret = LIST_FRONT(pList);
	list_pop_front(pList);
    return ret;
}

// player get N cards form draw pile
i32 draw_card(sGame *pGame, i32 player_id, size_t cnt) {
	if(pGame->draw_pile->size < cnt) return -1;  // no card to draw
	for(size_t i = 0; i < cnt; ++i) {
		i32 *dup_id = malloc(sizeof(i32));
		*dup_id = *(i32*)draw(pGame->draw_pile);
		list_push_back(pGame->players[player_id].cards, new_node(dup_id));
	}
	return 0;
}
