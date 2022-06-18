#include "damage.h"

void damage_event_default(sGame *pGame, sDamageEvent *e) {
    pGame->players[e->victim_id].hp -= e->damage;
}

// draw
void damage_event_bart_cassidy(sGame *pGame, sDamageEvent *e) {
    pGame->players[e->victim_id].hp -= e->damage;

    for(i32 i = 0; i < e->damage; ++i) {
        i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->victim_id].cards, card_id, true);
    }
}

void damage_event_el_gringo(sGame *pGame, sDamageEvent *e) {
    pGame->players[e->victim_id].hp -= e->damage;

    sList *card_list = pGame->players[e->damager_id].cards;
    for(i32 i = 0; i < e->damage; ++i) {
        i32 hand_card_num  = card_list->size;
        char cards_opt[hand_card_num][8];
        for(int i = 0; i < hand_card_num; ++i ) {

            sprintf(cards_opt[i], "(%2d)", i);
        }
        i32 card_id;
        sSelectEvent card_select_event = select_event_with_arr(pGame, e->damager_id, 1, 1, cards_opt, hand_card_num, 8);
    
        i32 take_id = *(i32*)LIST_FRONT(card_select_event.select_res);
        card_id = take_card(pGame, card_list, take_id);
        if(card_id != -1) {
            give_card(pGame, pGame->players[e->victim_id].cards, card_id, true);
        }

        free_list(card_select_event.selections);
		free_list(card_select_event.select_res);
    }
}