#include "card_func.h"
#include "select.h"
#include "draw.h"

void card_miss(sGame *pGame, i32 player_id, i32 card_id ) {

}

void card_bang(sGame *pGame, i32 player_id, i32 card_id ) {
	// select who to bang
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
	// call bang_event
	bang_event( pGame, player_id, target_id );
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}


void card_saldon(sGame *pGame, i32 player_id, i32 card_id ) {
	//幫大家加一滴血
	if( player_id == 0 ){
		printf("你幫大家加了回了一滴血!\n");
	}else printf(" -> %d 號玩家幫大家回了一滴血\n",player_id);

	LIST_FOR_EACH(pNode,pGame->live_players){
		i32 id = *(i32 *)pNode->data;
		i32 cur_hp = pGame->players[id].hp;
		i32 character_id = pGame->players[id].character;
		i32 highest_hp = characters[ character_id ].hp;
		
		if( cur_hp<highest_hp ) pGame->players[ id ].hp += 1;
	}
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_general_store(sGame *pGame, i32 player_id, i32 card_id ) {
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
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_panic(sGame *pGame, i32 player_id){

}

void card_stagecoach(sGame *pGame, i32 player_id, i32 card_id ) {
	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_array_id;
	for(i32 i = 0; i < 2; ++i) {
		card_array_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_array_id, true);
	}
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_wells_fargo(sGame *pGame, i32 player_id, i32 card_id ) {
	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_array_id;
	for(i32 i = 0; i < 3; ++i) {
		card_array_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_array_id, true);
	}
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_beer(sGame *pGame, i32 player_id, i32 card_id ) {
	if( (i32)pGame->live_players->size >= 2  ) {
		i32 id = *(i32 *)pGame->cur_player->data;
		pGame->players[id].hp += 1;
	}
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_cat_balou(sGame *pGame, i32 player_id, i32 card_id ) {
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
	i32 id = cards_id[select_idx];

	if(select_idx < (i32)target_desk->size) {
		take_card_by_id(pGame, target_desk, id);
	} else {
		take_card_by_id(pGame, target_hand, id);
	}

	give_card( pGame , pGame->discard_pile , id , true );

	printf("player %d 棄掉了 %s\n", target_id, cards[ id ].name);

	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_jail(sGame *pGame, i32 player_id, i32 card_id){
	if( player_id == 0 ){
		printf(" 你 使用了 監獄\n");
		printf(" 請選擇使用對象:\n");
	}else printf(" > player %d 使用了監獄\n",player_id);
	i32 select_idx = select_player( pGame, player_id );
	
	
	if( select_idx == 0 ){
		printf("你被關進監獄了QQ\n");
	}else printf(" > player %d 被關進監獄了QQ\n",select_idx);
	// 把jail放到某個人的桌上
	// 把jail從自己手牌移除
	give_card( pGame, pGame->players[select_idx].desk, card_id, true );
	take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}
void card_bomb(sGame *pGame, i32 player_id, i32 card_id){
	// 把bomb從手牌移走 是外面要做的事情
	if( player_id == 0 ){
		printf("你放了一個炸彈\n");
	}else printf("player %d 放了一個炸彈\n",player_id);

	give_card( pGame, pGame->players[player_id].desk, card_id, true );
	take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_gatlin(sGame *pGame, i32 player_id, i32 card_id ){
	// 從自己開始繞一圈，問每個人要不要丟miss，不要->扣血。 
	// 直接丟到棄牌堆

}
void card_indians(sGame *pGame, i32 player_id, i32 card_id ){
	// 從自己開始繞一圈，問每個人要不要丟bang，不要->扣血。 
	// 直接丟到棄牌堆
}
void card_duel(sGame *pGame, i32 player_id, i32 card_id ){
	// 用while()，跑你、我、你、我，直到偵測到throw_card回傳1，就break。 
	// 直接丟到棄牌堆
	if( player_id == 0 ){
		printf("你 丟出了 決鬥\n");
		printf("請選擇決鬥對象：");
	}else printf("> player %d 丟出了 決鬥\n");

	i32 duel_id=select_player(pGame, player_id);
	if ( duel_id==0 ){
		printf("你成為 player %d 的決鬥對象\n",player_id);
	}else printf("> player %d 成為 player %d 的決鬥對象");
}

void remove_card(sGame *pGame, i32 player_id, i32 target_card_type, i32 card_id ) {
    sList *live_player = pGame->live_players;
    sList *player_desk = pGame->players[player_id].desk;
    char buf[BUFSIZ];
    i32 pos = 0, take_id = 0;
    LIST_FOR_EACH(pNode, player_desk) {
        i32 card_id = *(i32*)pNode->data;
        if(cards[card_id].type == target_card_type) {
            take_id = take_card(pGame, pGame->players[player_id].desk, pos);
            if(take_id != -1) {
                give_card(pGame, pGame->discard_pile, card_id, true);
            }
        }
        pos++;
    }
}

void card_mustang(sGame *pGame, i32 player_id, i32 card_id ) {
    remove_card(pGame, player_id, 18, card_id);
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}
void card_scope(sGame *pGame, i32 player_id, i32 card_id) {
    remove_card(pGame, player_id, 17, card_id);
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}
void card_barrel(sGame *pGame, i32 player_id, i32 card_id) {
    remove_card(pGame, player_id, 20, card_id);
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

// guns
void card_remington(sGame *pGame, i32 player_id, i32 card_id) {
    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i, card_id);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}
void card_schofild(sGame *pGame, i32 player_id, i32 card_id) {
    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i, card_id);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}
void card_winchester(sGame *pGame, i32 player_id, i32 card_id) {
    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i, card_id);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}
void card_carabine(sGame *pGame, i32 player_id, i32 card_id) {
    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i, card_id);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}
void card_volcano(sGame *pGame, i32 player_id, i32 card_id) {
    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i, card_id);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}