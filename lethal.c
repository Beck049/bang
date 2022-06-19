#include"lethal.h"

#define BEER_TYPE 11

bool card_is_beer(i32 card_id) {
    return cards[card_id].type == BEER_TYPE;
}

void lethal_event_default(sGame *pGame, sLethalEvent *e) {
    sListNode *cur_p = get_player(pGame, e->target_id);
    sList *player_cards = pGame->players[e->target_id].cards;
    sList *beers = card_filter(player_cards, card_is_beer);

    i32 recover = pGame->players[*(i32*)cur_p->data].hp;
    recover *= (-1);
    recover += 1;

    if((i32)beers->size < recover) {
        list_erase( pGame->live_players, cur_p );
		free_list(beers);
		return;
	}
    i32 beer_id[beers->size];

    sListNode *cur_node = LIST_BEGIN(beers);
	for(i32 i = 0; i < (i32)beers->size; ++i) {
		beer_id[i] = *(i32*)cur_node->data;
	}
    char options[beers->size][512];
    cur_node = LIST_BEGIN(beers);
    for(i32 i = 0; i < (i32)beers->size; ++i) {
        i32 card_id = *(i32*)cur_node->data;
        sprintf(options[i], "%2d) %s (id: %d):\n%s", i, cards[card_id].name, card_id, cards[card_id].description);
    }
    sSelectEvent select_leth_e = select_event_with_arr(pGame, e->target_id, recover, recover, options, beers->size, sizeof(*options));
    
    LIST_FOR_EACH(pNode, select_leth_e.select_res){
        i32 select_card_id = beer_id[*(i32*)pNode->data];
        i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
        if(take_id != -1) {
            give_card(pGame, pGame->discard_pile, take_id, true);
        }
    }
    free_list(select_leth_e.selections);
    free_list(select_leth_e.select_res);

	free_list(beers);

    pGame->players[*(i32*)cur_p->data].hp = 1;
}

void lethal_event_sid_ketchum(sGame *pGame, sLethalEvent *e) {
    sListNode *cur_p = get_player(pGame, e->target_id);
    sList *player_cards = pGame->players[e->target_id].cards;
    sList *beers = card_filter(player_cards, card_is_beer);

    i32 hand_card_num = pGame->players[e->target_id].cards->size;
    i32 recover = pGame->players[*(i32*)cur_p->data].hp;
    recover *= (-1); recover += 1;
    i32 max_recover = beers->size + (hand_card_num - beers->size) /2;

    if(max_recover < recover) {
        list_erase( pGame->live_players, cur_p );
		free_list(beers);
		return;
	}
    
    // beer
    i32 beer_id[beers->size];
    sListNode *cur_node = LIST_BEGIN(beers);
	for(i32 i = 0; i < (i32)beers->size; ++i) {
		beer_id[i] = *(i32*)cur_node->data;
	}
    char options[beers->size][512];
    cur_node = LIST_BEGIN(beers);
    for(i32 i = 0; i < (i32)beers->size; ++i) {
        i32 card_id = *(i32*)cur_node->data;
        sprintf(options[i], "%2d) %s (id: %d):\n%s", i, cards[card_id].name, card_id, cards[card_id].description);
    }

    i32 chose_num = min(recover, (i32)beers->size);
    sSelectEvent select_leth_e = select_event_with_arr(pGame, e->target_id, chose_num, chose_num, options, beers->size, sizeof(*options));
    LIST_FOR_EACH(pNode, select_leth_e.select_res){
        i32 select_card_id = beer_id[*(i32*)pNode->data];
        i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
        if(take_id != -1) {
            give_card(pGame, pGame->discard_pile, take_id, true);
        }
    }
    free_list(select_leth_e.selections);
    free_list(select_leth_e.select_res);
    // discard 2 card
    i32 left_size = pGame->players[*(i32*)cur_p->data].cards->size;
    i32 left_card_id[left_size];
    cur_node = LIST_BEGIN(pGame->players[*(i32*)cur_p->data].cards);
	for(i32 i = 0; i < left_size; ++i) {
		left_card_id[i] = *(i32*)cur_node->data;
	}
    char left_options[left_size][512];
    cur_node = LIST_BEGIN(pGame->players[*(i32*)cur_p->data].cards);
    for(i32 i = 0; i < left_size; ++i) {
        i32 card_id = *(i32*)cur_node->data;
        sprintf(left_options[i], "%2d) %s (id: %d):\n%s", i, cards[card_id].name, card_id, cards[card_id].description);
    }

    chose_num = recover*2;
    sSelectEvent select_discard2_e = select_event_with_arr(pGame, e->target_id, chose_num, chose_num, left_options, left_size, sizeof(*left_options));
    
    LIST_FOR_EACH(pNode, select_discard2_e.select_res) {
        i32 select_card_id = left_card_id[*(i32*)pNode->data];
        i32 take_id = take_card_by_id(pGame, player_cards, select_card_id);
        if(take_id != -1) {
            give_card(pGame, pGame->discard_pile, take_id, true);
        }
    }
    free_list(select_discard2_e.selections);
    free_list(select_discard2_e.select_res);

	free_list(beers);

    pGame->players[*(i32*)cur_p->data].hp = 1;
}