#include "damage.h"

void damage_event_default(sGame *pGame, sDamageEvent *e) {
    pGame->players[e->victim_id].hp -= e->damage;
    if(e->victim_id == 0) {
        printf("你 受到 %d 點傷害，血量剩餘: %d\n", e->damage, pGame->players[e->victim_id].hp);
    } else {
        printf("玩家 %d 受到 %d 點傷害，血量剩餘: %d\n", e->victim_id, e->damage, pGame->players[e->victim_id].hp);
    }
}

// draw
void damage_event_bart_cassidy(sGame *pGame, sDamageEvent *e) {
    damage_event_default(pGame, e);

    if(e->victim_id == 0) {
        printf("[能力] 你 從排堆抽出 %d 張牌加到自己手上\n", e->damage);
    } else {
        printf("[能力] 玩家 %d 從排堆抽出 %d 張牌加到自己手上\n", e->victim_id, e->damage);
    }
    for(i32 i = 0; i < e->damage; ++i) {
        i32 card_id = take_card(pGame, pGame->draw_pile, 0);
		if(card_id == -1) break;
		give_card(pGame, pGame->players[e->victim_id].cards, card_id, true);
    }
}

void damage_event_el_gringo(sGame *pGame, sDamageEvent *e) {
    damage_event_default(pGame, e);
    char victim_appellation[16], damager_appellation[16];

    if(e->damager_id == -1) return;

    if(e->victim_id == 0) {
		sprintf(victim_appellation, "你");
	}
	else {
		sprintf(victim_appellation, "玩家 %d", e->victim_id);
	}
	if(e->damager_id == 0) {
		sprintf(damager_appellation, "你");
	}
	else {
		sprintf(damager_appellation, "玩家 %d", e->damager_id);
	}

    printf("[能力] %s 可以從 %s 手上抽出 %d 張牌加到自己手上\n", victim_appellation, damager_appellation, e->damage);

    sList *card_list = pGame->players[e->damager_id].cards;
    for(i32 i = 0; i < e->damage; ++i) {
        i32 hand_card_num  = card_list->size;

        if(hand_card_num == 0) {
            if(e->damager_id == 0) {
                printf("哈哈我沒有牌可以讓你抽了> <\n");
            } else {
                printf("很可惜的是，玩家 %d 手上已經沒牌了\n", e->damager_id);
            }
            return;
        }

        char cards_opt[hand_card_num][512];
        for(int i = 0; i < hand_card_num; ++i ) {
            sprintf(cards_opt[i], "%2d) 手牌 %d", i+1, i+1);
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