#include "dodge.h"
#define MISS_TYPE 0
#define BANG_TYPE 1

bool card_is_missed(i32 card_id) {
	return cards[card_id].type == MISS_TYPE;
}
bool card_is_bang(i32 card_id) {
	return cards[card_id].type == BANG_TYPE;
}

void dodge_event_default(sGame *pGame, sDodgeEvent *e) {
	i32 dodge_times = e->dodge_times;
	sList *player_cards = pGame->players[e->target_id].cards;
	sList *miss_cards = card_filter(player_cards, card_is_missed);
	if(miss_cards->size == 0) {
		free_list(miss_cards);
		return;
	}
	i32 miss_card_id[miss_cards->size];
	sListNode *cur_node = LIST_BEGIN(miss_cards);
	for(i32 i = 0; i < (i32)miss_cards->size; ++i) {
		miss_card_id[i] = *(i32*)cur_node->data;
	}
	char options[2][512] = {" 1) use missed", " 2) no"};
	sSelectEvent select_dodge_e = select_event_with_arr (pGame, e->target_id, 1, 1, options, 2, sizeof(*options));
	i32 choice = *(i32*)LIST_FRONT(select_dodge_e.select_res);
	if(choice == 0) {
		char options[miss_cards->size][128];
		sListNode *cur_node = LIST_BEGIN(miss_cards);
		for(i32 i = 0; i < (i32)miss_cards->size; ++i) {
			i32 card_id = *(i32*)cur_node->data;
			sprintf(options[i], "%2d) %s", i+1, cards[card_id].name, card_id);
		}
		sSelectEvent select_miss_e = select_event_with_arr(pGame, e->target_id, dodge_times, dodge_times, options, miss_cards->size, 128);
		i32 select_card_id = miss_card_id[*(i32*)LIST_FRONT(select_miss_e.select_res)];
		i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
		if(take_id != -1) {
			give_card(pGame, pGame->discard_pile, take_id, true);
		}
		free_list(select_miss_e.selections);
		free_list(select_miss_e.select_res);
	}
	free_list(select_dodge_e.selections);
	free_list(select_dodge_e.select_res);
	free_list(miss_cards);
}

void dodge_event_calamity_janet(sGame *pGame, sDodgeEvent *e) {
	i32 dodge_times = e->dodge_times;
	sList *player_cards = pGame->players[e->target_id].cards;
	sList *miss_cards = card_filter(player_cards, card_is_missed);
	sList *bang_cards = card_filter(player_cards, card_is_bang);
	// concat two sList
	LIST_FOR_EACH(pNode, bang_cards) {
		list_push_back(miss_cards, pNode);
	}

	if(miss_cards->size == 0) {
		free_list(miss_cards);
		return;
	}
	i32 miss_card_id[miss_cards->size];
	sListNode *cur_node = LIST_BEGIN(miss_cards);
	for(i32 i = 0; i < (i32)miss_cards->size; ++i) {
		miss_card_id[i] = *(i32*)cur_node->data;
	}
	char options[2][16] = {" 1) use missed", " 2) no"};
	sSelectEvent select_dodge_e = select_event_with_arr(pGame, e->target_id, 1, 1, options, 2, sizeof(*options));
	i32 choice = *(i32*)LIST_FRONT(select_dodge_e.select_res);
	if(choice == 0) {
		char options[miss_cards->size][128];
		sListNode *cur_node = LIST_BEGIN(miss_cards);
		for(i32 i = 0; i < (i32)miss_cards->size; ++i) {
			i32 card_id = *(i32*)cur_node->data;
			sprintf(options[i], "%2d) %s", i+1, cards[card_id].name, card_id);
		}
		sSelectEvent select_miss_e = select_event_with_arr(pGame, e->target_id, dodge_times, dodge_times, options, miss_cards->size, 128);
		i32 select_card_id = miss_card_id[*(i32*)LIST_FRONT(select_miss_e.select_res)];
		i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
		if(take_id != -1) {
			give_card(pGame, pGame->discard_pile, take_id, true);
		}
		free_list(select_miss_e.selections);
		free_list(select_miss_e.select_res);
	}
	free_list(select_dodge_e.selections);
	free_list(select_dodge_e.select_res);
	free_list(miss_cards);
	free_list(bang_cards);
}

void dodge_event_barrel(sGame *pGame, sDodgeEvent *e) {
	sDetermineEvent dtm_e = determine_event(pGame, e->target_id);
	if(cards[dtm_e.determine_res].suit == HEART) {
		e->dodge_res = true;
	}
}