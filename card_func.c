#include "card_func.h"

void (*card_funcs[80])(sGame *pGame, i32 player_id);

void card_miss(sGame *pGame, i32 player_id) {

}

void card_bang(sGame *pGame, i32 player_id) {

}

void card_general_store(sGame *pGame, i32 player_id) {
	i32 arr[10] = {0};
	i32 num = pGame->live_players->size;
	sListNode *cur_p = get_player(pGame, player_id);
	for(i32 i = 0; i < num; ++i) {
		arr[i] = take_card(pGame, pGame->draw_pile, 0);
	}
	char cards_opt[num][32];
	for(int i = 0; i < num; ++i ) {

		sprintf(cards_opt[i], "pick a card : %d", i);
	}
	for(i32 i = 0; i < num; ++i ) {
		// draw
		// print all card
		sSelectEvent event = select_event_with_arr(pGame, *(i32 *)cur_p->data , 1, 1, cards_opt, num-i, 32);
		i32 take_id = *(i32*)LIST_FRONT(event.select_res);
		i32 card_id = arr[take_id];
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);

		strcpy(cards_opt[take_id], cards_opt[num-i-1]);

		cur_p = cur_p->next;
	}
}

void card_stagecoach(sGame *pGame, i32 player_id) {
	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_id;
	for(i32 i = 0; i < 2; ++i) {
		card_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);
	}
}

void card_wells_fargo(sGame *pGame, i32 player_id) {
	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_id;
	for(i32 i = 0; i < 3; ++i) {
		card_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);
	}
}

void card_beer(sGame *pGame, i32 player_id) {
	if( (i32)pGame->live_players->size >= 2  ) {
		i32 id = *(i32 *)pGame->cur_player->data;
		pGame->players[id].hp += 1;
	}
}

void card_cat_balou(sGame *pGame, i32 player_id) {
	printf("您將使用 Cat balou此張卡片!\n");
	printf("請選擇一位玩家，其將自行選擇棄掉任何一張牌。\n");
	//選人，除了自己。
	i32 live_size = pGame->live_players->size;
	i32 players_id[live_size];
	char players_option[live_size][16];

	int cnt=0;
	LIST_FOR_EACH(pNode, pGame->live_players) {
		i32 id = *(i32 *)pNode->data;
		if(id  != player_id) {
			players_id[cnt] = id;
			sprintf(players_option[cnt], "%d) player%d\n", cnt+1, id);
			++cnt;
		}
	}
	
	sSelectEvent sl_e = select_event_with_arr(pGame, player_id, 1, 1, players_option, cnt, sizeof(*players_option));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	i32 target_id =  players_id[select_idx];
	free_list(sl_e.select_res);
	free_list(sl_e.selections);

	printf("您選擇了： %d \n", target_id);

	//叫那個人給我丟掉一張牌喔
	sList *target_desk = pGame->players[ target_id ].desk;
	sList *target_hand = pGame->players[ target_id ].cards;
	i32 target_cards_cnt = target_desk->size + target_hand->size;
	i32 cards_id[target_cards_cnt];
	char cards_option[target_cards_cnt][512];

	cnt = 0;
	LIST_FOR_EACH(pNode, target_desk){
		i32 id = *(i32*)pNode->data;
		cards_id[cnt] = id;
		sprintf(cards_option[cnt], "%d) %s (desk)\n", cnt+1, cards[id].name);
		++cnt;
	}
	LIST_FOR_EACH(pNode, target_hand){
		i32 id = *(i32*)pNode->data;
		cards_id[cnt] = id;
		sprintf(cards_option[cnt], "%d) %s (hand)\n", cnt+1, cards[id].name);
		++cnt;
	}

	sl_e = select_event_with_arr(pGame, target_id, 1, 1, cards_option, cnt , sizeof(*cards_option));
	select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	i32 card_id = cards_id[select_idx];

	if(select_idx < target_desk->size) {
		take_card_by_id(pGame, target_desk, card_id);
	} else {
		take_card_by_id(pGame, target_hand, card_id);
	}

	give_card( pGame , pGame->discard_pile , card_id , true );

	printf("player %d 棄掉了 %s\n", target_id, cards[ card_id ].name);
}


void init_card_funcs() {
	card_funcs[0] = &card_miss;
	card_funcs[1] = &card_bang;
};