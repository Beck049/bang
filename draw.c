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
				if(extra_card_id != -1) {
					give_card(pGame, pGame->players[e->target_id].cards, extra_card_id, true);
				}
			}
		}
	}
}
	
void draw_phase_event_jesse_jones(sGame *pGame, sDrawPhaseEvent *e) {
	
	for(i32 i = 0; i < 2; ++i) {
		// draw 1 card from draw_pile
		i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
		if( i == 1 ) {
			// select draw first card from draw_pile or player
			sSelectEvent event = select_event(pGame, e->target_id, 1, 1, "1) draw from deck pile", "2) draw from player");
			if( *(i32*)LIST_FRONT(event.select_res) == 1 )
			{
				card_id = take_card(pGame, pGame->draw_pile, 0);
				if(card_id == -1) break;
				give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
			}
			else
			{
				// sSelectEvent choose player
					// sSelectEvent choose card from player
				// card_id = take_card(pGame, //sList *src// , 0);
				if(card_id == -1) break;
				give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
			}
			// free
			free(event.selections);
			free(event.select_res);
		}
	}
}
	
void draw_phase_event_kit_carlson(sGame *pGame, sDrawPhaseEvent *e) {
	// draw 3 card from draw_pile
	i32 card_id[3];
	for(i32 i = 0; i < 3; ++i ) {
		card_id[i] = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
	}
	// select two of them
	char* str[3];
	char *card_1 = "1) ", *card_2 = "2) ", *card_3 = "3) ";
	str[0] = card_1; str[1] = card_2; str[2] = card_3;

	for(int i = 0; i < 3; ++i) {
		strcat(str[i], cards[card_id[0]].name); 
		strcat(str[i], ":\n");
		strcat(str[i], cards[card_id[0]].description);
	}

	sSelectEvent event = select_event(pGame, e->target_id, 2, 2, card_1, card_2, card_3);
	// push the other one onto top of draw_pile
	LIST_FOR_EACH(pNode, event.select_res) {
		give_card(pGame, pGame->players[e->target_id].cards, card_id[(i32)pNode->data], true);
		card_id[(i32)pNode->data] = -1;
	}
	LIST_FOR_EACH(pNode, event.select_res) {
		if(card_id[(i32)pNode->data] != -1)
		{
			list_push_front( pGame->draw_pile, pNode );
		}
	}
	// free
	for(int i = 0; i < 3; ++i)
	{
		free(str[i]);
	}
	free(event.selections);
	free(event.select_res);
}
	
void draw_phase_event_pedro_ramirez(sGame *pGame, sDrawPhaseEvent *e) {
	
	for(i32 i = 0; i < 2; ++i) {
		// draw 1 card from draw_pile
		i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
		if( i == 1 ) {
			// select draw first card from draw_pile or discard_pile
			sSelectEvent event = select_event(pGame, e->target_id, 1, 1, "1) draw from deck pile", "2) draw from discard pile");
			if( *(i32*)LIST_FRONT(event.select_res) == 1 )
			{
				card_id = take_card(pGame, pGame->draw_pile, 0);
				if(card_id == -1) break;
				give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
			}
			else
			{
				card_id = take_card(pGame, pGame->discard_pile, 0);
				if(card_id == -1) break;
				give_card(pGame, pGame->players[e->target_id].cards, card_id, true);
			}
			// free
			free(event.selections);
			free(event.select_res);
		}
	}
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
