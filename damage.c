#include "damage.h"

void damage_event_default(sGame *pGame, sDamageEvent *e) {
    pGame->players[e->victim_id].hp -= e->damage;
    printf("Player%d 受到 %d 點傷害，血量剩餘: %d\n", e->victim_id, e->damage, pGame->players[e->victim_id].hp);
}

// draw
void damage_event_bart_cassidy(sGame *pGame, sDamageEvent *e) {
    damage_event_default(pGame, e);

    printf("Player%d 可以從排堆抽出 %d 張牌加到自己手上\n", e->victim_id, e->damage);
    for(i32 i = 0; i < e->damage; ++i) {
        i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->victim_id].cards, card_id, true);
    }
}

void damage_event_el_gringo(sGame *pGame, sDamageEvent *e) {
    damage_event_default(pGame, e);

    if(e->damager_id == -1) return;

    printf("Player%d 可以從 player %d 手上抽出 %d 張牌加到自己手上\n", e->victim_id, e->damager_id, e->damage);
    sList *card_list = pGame->players[e->damager_id].cards;
    for(i32 i = 0; i < e->damage; ++i) {
        i32 hand_card_num  = card_list->size;
        char cards_opt[hand_card_num][512];
        for(int i = 0; i < hand_card_num; ++i ) {
            sprintf(cards_opt[i], "%2d) player%d 的手牌%d", i+1, e->damager_id, i+1);
        }
        i32 card_id;
        sSelectEvent card_select_event = select_event_with_arr(pGame, e->victim_id, 1, 1, cards_opt, hand_card_num, sizeof(*cards_opt));
    
        i32 take_id = *(i32*)LIST_FRONT(card_select_event.select_res);
        card_id = take_card(pGame, card_list, take_id);
        if(card_id != -1) {
            give_card(pGame, pGame->players[e->victim_id].cards, card_id, true);
        }

        free_list(card_select_event.selections);
		free_list(card_select_event.select_res);
    }
}