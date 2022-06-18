#include "discard.h"

void discard(sGame *pGame, i32 player_id){
	sPlayer *player = &pGame->players[player_id];
	i32 size = player->cards->size;
	i32 hp = player->hp;

	// no need to discard
	if(size <= hp) return;

	i32 cards_id[size];
	char selections[size][16];

	i32 cnt = 0;
	LIST_FOR_EACH(pNode, player->cards) {
		//所有手牌都可選擇要不要棄，所以把所有的牌都寫進table裡面。
		i32 card_id = *(i32*)pNode->data;
		cards_id[cnt] = card_id;
		sprintf(selections[cnt], "%d) %s", cnt+1, cards[card_id].name);
		++cnt;
	}

	sSelectEvent sl_e = select_event_with_arr(pGame, player_id, size-hp, size-hp, selections, size, 16);

	LIST_FOR_EACH(pNode, sl_e.select_res){
		i32 selected_idx = *(i32*)pNode->data;
		i32 card_id = cards_id[selected_idx];
		take_card_by_id(pGame, player->cards, card_id);
		give_card( pGame, pGame->discard_pile, card_id, false);
	}
	free(sl_e.selections);
	free(sl_e.select_res);
}