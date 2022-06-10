#include "draw.h"

void swap_pile(sList *pListA, sList *pListB) {
	sListNode *tmp = pListA->end;
	pListA->end = pListB->end;
	pListB->end = tmp;
	pListA->size ^= pListB->size;
	pListB->size ^= pListA->size;
	pListA->size ^= pListB->size;
}

i32 take_card(sGame *pGame, sList *src, i32 pos) {
	if(!(pos >= 0 && pos < (i32)src->size)) return -1; // take fail
	sListNode *target = LIST_BEGIN(src);
	for(i32 i = 0; i < pos; ++i) target = target->next;
	list_erase(src, target);
	// src is draw pile and it is empty
	if(src == pGame->draw_pile && src->size == 0) {
		swap_pile(pGame->draw_pile, pGame->discard_pile);
		shuffle(pGame->draw_pile);
	}
	return *(i32*)target->data;
}

void give_card(sGame *pGame, sList *dst, i32 card_id, bool tail_insert) {
	i32 *data = malloc(sizeof(i32));
	*data = card_id;
	if(tail_insert) list_push_back(dst, new_node(data));
	else list_push_front(dst, new_node(data));
}

void draw_phase_event_default(sGame *pGame, sDrawPhaseEvent *e) {
	// draw 2 card from draw_pile
	for(i32 i = 0; i < 2; ++i) {
		i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
	}
}

void draw_phase_event_black_jack(sGame *pGame, sDrawPhaseEvent *e) {
	for(i32 i = 0; i < 2; ++i) {
		i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->target_id].cards, card_id, true);

		if(i == 1) {
			// TODO: show second card to everyone
			if(cards[card_id].suit == HEART || cards[card_id].suit == DIAMOND) {
				i32 extra_card_id = take_card(pGame, pGame->draw_pile, 0);
				give_card(pGame, pGame->players[e->target_id].cards, extra_card_id, true);
			}
		}
	}
	
}	
	
void draw_phase_event_jesse_jones(sGame *pGame, sDrawPhaseEvent *e) {
	
	
	// select draw first card from draw_pile or player
	// draw 1 card from draw_pile
}
	
void draw_phase_event_kit_carlson(sDrawPhaseEvent *e) {
	// draw 3 card from draw_pile
	// select two of them
	// push the other one onto top of draw_pile
}
	
void draw_phase_event_pedro_ramirez(sDrawPhaseEvent *e) {
	// select draw first card from draw_pile or discard_pile
	// draw 1 card from draw_pile
}

// player get N cards form draw pile
// i32 draw_card(sGame *pGame, i32 player_id, size_t cnt) {
// 	if(pGame->draw_pile->size < cnt) return -1;  // no card to draw
// 	for(size_t i = 0; i < cnt; ++i) {
// 		i32 *dup_id = malloc(sizeof(i32));
// 		*dup_id = *(i32*)draw(pGame->draw_pile);
// 		list_push_back(pGame->players[player_id].cards, new_node(dup_id));
// 	}
// 	return 0;
// }
