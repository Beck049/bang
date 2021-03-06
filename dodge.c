#include "dodge.h"
#include "display.h"

bool card_is_missed(i32 card_id) {
	return cards[card_id].type == MISS;
}
bool card_is_bang(i32 card_id) {
	return cards[card_id].type == BANG;
}

void dodge_event_default(sGame *pGame, sDodgeEvent *e) {
	i32 dodge_times = e->dodge_times;
	sList *player_cards = pGame->players[e->target_id].cards;
	sList *miss_cards = card_filter(player_cards, card_is_missed);

	if((i32)miss_cards->size < dodge_times) {
		free_list(miss_cards);
		return;
	}

	char options[2][512] = {" 1) 丟出 miss 以進行躲避", " 2) 不進行躲避"};
	sSelectEvent select_dodge_e = select_event_with_arr (pGame, e->target_id, 1, 1, options, 2, sizeof(*options));
	i32 choice = *(i32*)LIST_FRONT(select_dodge_e.select_res);
	free_list(select_dodge_e.selections);
	free_list(select_dodge_e.select_res);

	if(choice == 0) {
		if(e->target_id == 0) {
			printf("你選擇進行躲避\n");
		} else printf("玩家 %d 選擇進行躲避\n", e->target_id);

		i32 miss_card_id[miss_cards->size];
		char options_2[miss_cards->size][512];

		sListNode *cur_node = LIST_BEGIN(miss_cards);
		for(i32 i = 0; i < (i32)miss_cards->size; ++i) {
			i32 card_id = *(i32*)cur_node->data;
			miss_card_id[i] = *(i32*)cur_node->data;
			sprintf(options_2[i], "%2d) %s", i+1, cards[card_id].name);
			cur_node = cur_node->next;
		}

		sSelectEvent select_miss_e = select_event_with_arr(
			pGame, e->target_id, dodge_times, dodge_times,
			options_2, miss_cards->size, sizeof(*options_2));

		LIST_FOR_EACH(pNode, select_miss_e.select_res) {
			i32 select_card_id = miss_card_id[*(i32*)pNode->data];
			i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
			if(take_id != -1) {
				give_card(pGame, pGame->discard_pile, take_id, false);
			}
		}
		printf(YLW"-> 躲避成功\n"RST);
		e->dodge_res = true;
		free_list(select_miss_e.selections);
		free_list(select_miss_e.select_res);
	}
	free_list(miss_cards);
}

void dodge_event_calamity_janet(sGame *pGame, sDodgeEvent *e) {
	i32 dodge_times = e->dodge_times;
	sList *player_cards = pGame->players[e->target_id].cards;
	sList *miss_cards = card_filter(player_cards, card_is_missed);
	sList *bang_cards = card_filter(player_cards, card_is_bang);
	// concat two sList
	while((i32) bang_cards->size > 0) {
		i32 card_id = take_card(pGame, bang_cards, 0);
		give_card(pGame, miss_cards, card_id, true);
	}

	if((i32)miss_cards->size < e->dodge_times) {
		free_list(bang_cards);
		free_list(miss_cards);
		return;
	}

	char options[2][512] = {" 1) 丟出 miss 以進行躲避", " 2) 不進行躲避"};
	sSelectEvent select_dodge_e = select_event_with_arr(pGame, e->target_id, 1, 1, options, 2, sizeof(*options));
	i32 choice = *(i32*)LIST_FRONT(select_dodge_e.select_res);
	free_list(select_dodge_e.selections);
	free_list(select_dodge_e.select_res);

	if(choice == 0) {
		if(e->target_id == 0) {
			printf("你選擇進行躲避\n");
		} else printf("玩家 %d 選擇進行躲避\n", e->target_id);


		i32 miss_card_id[miss_cards->size];
		char options_2[miss_cards->size][512];

		sListNode *cur_node = LIST_BEGIN(miss_cards);
		for(i32 i = 0; i < (i32)miss_cards->size; ++i) {
			i32 card_id = *(i32*)cur_node->data;
			miss_card_id[i] = *(i32*)cur_node->data;
			sprintf(options_2[i], "%2d) %s", i+1, cards[card_id].name);
			cur_node = cur_node->next;
		}

		sSelectEvent select_miss_e = select_event_with_arr(
			pGame, e->target_id, dodge_times, dodge_times,
			options_2, miss_cards->size, sizeof(*options_2));

		LIST_FOR_EACH(pNode, select_miss_e.select_res) {
			i32 select_card_id = miss_card_id[*(i32*)pNode->data];
			i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
			if(take_id != -1) {
				give_card(pGame, pGame->discard_pile, take_id, false);
			}
		}
		printf(YLW"-> 躲避成功\n"RST);
		e->dodge_res = true;
		free_list(select_miss_e.selections);
		free_list(select_miss_e.select_res);
	}
	free_list(miss_cards);
	free_list(bang_cards);
}

void dodge_event_barrel(sGame *pGame, sDodgeEvent *e) {
	sDetermineEvent dtm_e = determine_event(pGame, e->target_id);
	if(cards[dtm_e.determine_res].suit == HEART) {
		e->dodge_res = true;
		printf(YLW"-> 躲避成功\n"RST);
	}
}