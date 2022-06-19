#include "card_func.h"
#include "select.h"
#include "draw.h"

void card_miss(sGame *pGame, i32 player_id, i32 card_id ) {

}

void card_bang(sGame *pGame, i32 player_id, i32 card_id ) {
	// select who to bang
	if( player_id == 0 ){
		printf(" 你使用了 bang \n");
		printf(" 請選擇bang的對象: ");
	}else printf("> player %d 使用了bang\n",player_id);

	i32 live_size = pGame->live_players->size;
	i32 players_id[live_size];
	char players_option[live_size][512];

	int cnt=0;
	LIST_FOR_EACH(pNode, pGame->live_players) {
		i32 id = *(i32 *)pNode->data;
		if(id  != player_id) {
			players_id[cnt] = id;
			sprintf(players_option[cnt], "%d) player%d", cnt+1, id);
			++cnt;
		}
	}
	
	sSelectEvent sl_e = select_event_with_arr(pGame, player_id, 1, 1, players_option, cnt, sizeof(*players_option));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	i32 target_id =  players_id[select_idx];
	free_list(sl_e.select_res);
	free_list(sl_e.selections);
	if( target_id == 0 ){
		printf("你被bang了QAQ\n\n");
	}else printf("> player %d 被bang了\n\n",target_id);

	// call bang_event
	bang_event( pGame, player_id, target_id );
	damage_event( pGame, target_id , player_id, 1 );
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
	if( player_id == 0 ){
		printf("你使用了雜貨店\n");
	}else printf("> player %d 使用了雜貨店\n",player_id);

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
		sSelectEvent event = select_event_with_arr(pGame, *(i32 *)cur_p->data , 1, 1, cards_opt, num-i, sizeof(*cards_opt) );
		i32 take_id = *(i32*)LIST_FRONT(event.select_res);
		i32 card_id = arr[take_id];
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_id, true);

		strcpy(cards_opt[take_id], cards_opt[num-i-1]);

		cur_p = cur_p->next;
	}
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_panic(sGame *pGame, i32 player_id, i32 card_id){
	if( player_id == 0 ){
		printf("你 使用了 驚慌!\n");
	}else printf("> player %d 使用了 驚慌!\n",player_id);

	sListNode *cur_player = get_player(pGame, player_id);
	char players_option[5][512];
	i32 opt_num = 0;
	i32 dis = 0;
	LIST_FOR_EACH(pNode2, pGame->players[player_id].desk) {
		i32 id = *(i32*)pNode2->data;
		i32 type = cards[id].type;
		if( type == 17 ) {
			dis--;
		}
	}
	LIST_FOR_EACH(pNode, pGame->live_players) {
		i32 pos = node_distance( pGame->live_players, pNode, cur_player );
		if( pos > (i32)pGame->live_players->size /2 )
		{
			dis = (i32)pGame->live_players->size - pos;
		}
		i32 this_id = *(i32*)pNode->data;
		dis = dis + pGame->players[this_id].be_looked_range + pGame->players[player_id].look_range;
		LIST_FOR_EACH(pNode2, pGame->players[this_id].desk) {
			i32 id = *(i32*)pNode2->data;
			i32 type = cards[id].type;
			if( type == 18 ) {
				dis++;
			}
		}
	
		if( dis <= 1 ) {
			sprintf( players_option[opt_num], "pick a card from player %d", this_id ); 
			opt_num++;
		}
	}
	sSelectEvent panic_event = select_event_with_arr(pGame, player_id, 1, 1, players_option, opt_num, 32);
	i32 p_id = *(i32*)LIST_FRONT(panic_event.select_res);
	
	if( p_id == 0 ){
		printf("你被 驚慌了QQ\n");
	}else printf("> player %d 被驚慌了\n", p_id );

		i32 hand_card_num  = pGame->players[p_id].cards->size;
		i32 desk_card_num  = pGame->players[p_id].desk->size;
		i32 total_card_num = hand_card_num + desk_card_num;
		i32 select_card_id = 0;

		char cards_opt[total_card_num][512];
		for(int i = 0; i < total_card_num; ++i ) {
			sprintf(cards_opt[i], "(%2d)", i);
		}
		sSelectEvent card_select_event = select_event_with_arr(pGame, player_id, 1, 1, cards_opt, total_card_num, sizeof(*cards_opt));
		i32 take_id = *(i32*)LIST_FRONT(card_select_event.select_res);
		
		if(take_id < hand_card_num){
			select_card_id = take_card(pGame, pGame->players[p_id].cards, take_id);
		}else{
			select_card_id = take_card(pGame, pGame->players[p_id].desk, take_id - hand_card_num);
		}
		
		if( select_card_id != -1) {
			give_card(pGame, pGame->players[player_id].cards, select_card_id, true);
		}
		
		free_list(card_select_event.selections);
		free_list(card_select_event.select_res);

	free_list(panic_event.selections);
	free_list(panic_event.select_res);

	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_stagecoach(sGame *pGame, i32 player_id, i32 card_id ) {
	if( player_id == 0 ){
		printf("你使用了驛馬車\n");
	}else printf("> player %d 使用了驛馬車\n",player_id);

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
	if( player_id == 0 ){
		printf("你使用了 威爾斯法歌 \n");
	}else printf("> player %d 使用了威爾斯法歌\n",player_id);

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
		if( player_id == 0 ){
			printf(" 你 使用了 啤酒\n");
		}else printf("> player %d 使用了 啤酒 ",player_id);

		i32 id = *(i32 *)pGame->cur_player->data;
		i32 character_id = pGame->players[player_id].character;
		i32 highest_hp = characters[ character_id ].hp;

		if( pGame->players[id].hp<highest_hp ){
			pGame->players[id].hp += 1;
		}
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
	char players_option[live_size][512];

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
	if( player_id == 0 ){
		printf("你使用了格林機關槍\n");
	}else printf("> player %d 使用了格林機關槍\n",player_id);

    LIST_FOR_EACH(pNode, pGame->live_players) {
        i32 id = *(i32 *)pNode->data;
        if(id  != player_id) {
            i32 id_choose=select_throw( pGame, id, MISS );
            if( id_choose == -1 ){
                //不丟bang就扣血
                damage_event( pGame, id, player_id, 1 );
                if( id==0 ){
                    printf("你被扣一滴血了QQ\n");
                }else printf("> player %d 被扣了一滴血\n",id);
                break;
            }
            take_card_by_id( pGame, pGame->players[id].cards, id_choose );
            give_card( pGame, pGame->discard_pile, id_choose, true );
        }
    }

    give_card( pGame, pGame->players[player_id].desk, card_id, true );
    take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_indians(sGame *pGame, i32 player_id, i32 card_id ){
    // 從自己開始繞一圈，問每個人要不要丟bang，不要->扣血。 
    // 直接丟到棄牌堆
	if( player_id == 0 ){
		printf("你使用了 印地安人 \n");
	}else printf("> player %d 使用了印地安人\n",player_id);

    LIST_FOR_EACH(pNode, pGame->live_players) {
        i32 id = *(i32 *)pNode->data;
        if(id  != player_id) {
            i32 id_choose=select_throw( pGame, id, BANG );
            if( id_choose == -1 ){
                //不丟bang就扣血
                damage_event( pGame, id, player_id, 1 );
                if( id==0 ){
                    printf("你被扣一滴血了QQ\n");
                }else printf("> player %d 被扣了一滴血\n",id);
                break;
            }
            take_card_by_id( pGame, pGame->players[id].cards, id_choose );
            give_card( pGame, pGame->discard_pile, id_choose, true );
        }
    }

    give_card( pGame, pGame->players[player_id].desk, card_id, true );
    take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_duel(sGame *pGame, i32 player_id, i32 card_id ){
	// 用while()，跑你、我、你、我，直到偵測到throw_card回傳-1，就break。 
	// 直接丟到棄牌堆
	if( player_id == 0 ){
		printf("你 丟出了 決鬥\n");
		printf("請選擇決鬥對象：");
	}else printf("> player %d 丟出了 決鬥\n",player_id);

	i32 duel_id=select_player(pGame, player_id);
	if ( duel_id==0 ){
		printf("你成為 player %d 的決鬥對象\n",player_id);
	}else printf("> player %d 成為 player %d 的決鬥對象",duel_id,player_id);
	
	while(1){
		if( duel_id == 0 ) printf("您需要丟出一張bang,否則將會扣一滴血\n");
		i32 duel_id_choose=select_throw( pGame, duel_id, BANG );
		if( duel_id_choose == -1 ){
			//不丟bang就扣血
			//pGame->players[duel_id].hp--;阿姨不給機會:)
			damage_event( pGame, duel_id, player_id, 1 );

			if( duel_id==0 ){
				printf("你被扣一滴血了QQ\n");
			}else printf("> player %d 被扣了一滴血\n",duel_id);
			break;
		}
		take_card_by_id( pGame, pGame->players[duel_id].cards, duel_id_choose );
		give_card( pGame, pGame->discard_pile, duel_id_choose, true );

		if( player_id == 0 ) printf("您需要丟出一張bang,否則將會扣一滴血\n");
		i32 player_id_choose=select_throw( pGame, player_id, BANG );
		if( duel_id_choose == -1 ){
			//不丟bang就扣血
			//pGame->players[player_id].hp--;阿姨不給機會:)
			damage_event( pGame, duel_id, player_id, 1 );

			if( player_id==0 ){
				printf("你被扣一滴血了QAQ\n");
			}else printf("> player %d 被扣了一滴血\n",player_id);
			break;
		}
		take_card_by_id( pGame, pGame->players[player_id].cards, player_id_choose );
		give_card( pGame, pGame->discard_pile, player_id_choose, true );
	}
}

void remove_card(sGame *pGame, i32 player_id, i32 target_card_type) {
    // sList *live_player = pGame->live_players;
    sList *player_desk = pGame->players[player_id].desk;
    i32 pos = 0, take_id = 0;
    LIST_FOR_EACH(pNode, player_desk) {
        i32 card_id = *(i32*)pNode->data;
        if(((i32)cards[card_id].type) == target_card_type) {
            take_id = take_card(pGame, pGame->players[player_id].desk, pos);
            if(take_id != -1) {
                give_card(pGame, pGame->discard_pile, card_id, true);
            }
        }
        pos++;
    }
}

void card_mustang(sGame *pGame, i32 player_id, i32 card_id ) {
	if( player_id == 0 ){
		printf("你裝備了 野馬 \n");
	}else printf("> player %d 裝備了 野馬 \n",player_id);

    remove_card(pGame, player_id, 18);
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

void card_scope(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 望遠鏡瞄準器 \n");
	}else printf("> player %d 裝備了 望遠鏡瞄準器 \n",player_id);

    remove_card(pGame, player_id, 17);
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

void card_barrel(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 啤酒桶 \n");
	}else printf("> player %d 裝備了 啤酒桶 \n",player_id);

    remove_card(pGame, player_id, 20);
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

// guns
void card_remington(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 雷明頓 \n");
	}else printf("> player %d 裝備了 雷明頓 \n",player_id);

    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

void card_schofild(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 史考菲爾德 \n");
	}else printf("> player %d 裝備了 史考菲爾德 \n",player_id);

    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

void card_winchester(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 溫徹斯特 \n");
	}else printf("> player %d 裝備了 溫徹斯特 \n",player_id);

    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

void card_carabine(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 卡賓槍 \n");
	}else printf("> player %d 裝備了 卡賓槍 \n",player_id);

    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}

void card_volcano(sGame *pGame, i32 player_id, i32 card_id) {
	if( player_id == 0 ){
		printf("你裝備了 連發槍 \n");
	}else printf("> player %d 裝備了 連發槍 \n",player_id);

    for(int i = 12; i <= 16; ++i) {
        remove_card(pGame, player_id, i);
    }
	i32 take_id;
	take_id = take_card_by_id(pGame, pGame->players[player_id].cards, card_id);
	if(take_id != -1) {
		give_card(pGame, pGame->players[player_id].desk, card_id, true);
	}
}