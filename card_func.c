#include "card_func.h"
#include "select.h"
#include "draw.h"
#include "display.h"

void card_bang(sGame *pGame, i32 player_id, i32 card_id ) {
	// select who to bang
	if( player_id == 0 ){
		printf( YLW"->你使用了 bang \n"RST);
		printf(YLW"-> 請選擇bang的對象: "RST);
	}else printf(YLW"-> player %d 使用了bang\n"RST,player_id);

	i32 live_size = pGame->live_players->size;
	i32 players_id[live_size];
	char players_option[live_size][512];

	int cnt=0;
	LIST_FOR_EACH(pNode, pGame->live_players) {
		i32 id = *(i32 *)pNode->data;
		if(id  != player_id) {
			players_id[cnt] = id;
			sprintf(players_option[cnt], "%2d) player%d", cnt+1, id);
			++cnt;
		}
	}
	
	sSelectEvent sl_e = select_event_with_arr(pGame, player_id, 1, 1, players_option, cnt, sizeof(*players_option));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	i32 target_id =  players_id[select_idx];
	free_list(sl_e.select_res);
	free_list(sl_e.selections);
	if( target_id == 0 ){
		printf(YLW"-> 你被bang了QAQ\n\n"RST);
	}else printf(YLW"-> player %d 被bang了\n\n"RST,target_id);

	// call bang_event
	sBangEvent bang=bang_event( pGame, player_id, target_id );
	if( bang.bang_res==true ){
		damage_event(pGame, target_id, player_id, 1 );
		if(pGame->players[target_id].hp<=0){
			sLethalEvent lth_e = lethal_event(pGame, target_id);
			if(lth_e.lethal_res == true) {
				sDeathEvent dth_e = death_event(pGame, target_id, player_id);
				// cur_player died
				if(dth_e.death_res == true) {
					if( target_id == 0 ){
						printf(YLW"-> You died\n"RST);
					}else printf(YLW"-> player %d died"RST,target_id);
					return;
				}
			}
		}
	}
	printf(YLW"-> bang處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}


void card_saldon(sGame *pGame, i32 player_id, i32 card_id ) {
	//幫大家加一滴血
	if( player_id == 0 ){
		printf(YLW"-> 你幫大家加了回了一滴血!\n"RST);
	}else printf(YLW"-> %d 號玩家幫大家回了一滴血\n"RST,player_id);

	LIST_FOR_EACH(pNode,pGame->live_players){
		i32 id = *(i32 *)pNode->data;
		i32 cur_hp = pGame->players[id].hp;
		i32 character_id = pGame->players[id].character;
		i32 highest_hp = characters[ character_id ].hp;
		
		if( cur_hp<highest_hp ) pGame->players[ id ].hp += 1;
	}
	printf(YLW"-> 酒館處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_general_store(sGame *pGame, i32 player_id, i32 card_id ) {
	if( player_id == 0 ){
		printf(YLW"-> 你使用了雜貨店\n"RST);
	} else printf(YLW"-> player %d 使用了雜貨店\n"RST,player_id);


	// take `num` of card to pick
	i32 num = pGame->live_players->size;
	sList *pick_cards = new_list();
	for(i32 i = 0; i < num; ++i) {
		i32 take_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pick_cards, take_id, true);
		printf("%2d) %s\n", i+1, cards[take_id].name);
	}

	sListNode *cur_p = get_player(pGame, player_id);
	// loop everyone to pick
	while(pick_cards->size > 0) {
		i32 last_size = pick_cards->size;

		// cur_p to draw
		i32 cur_p_id = *(i32*)cur_p->data;

		i32 cards_id[last_size];
		char cards_opt[last_size][64];
		for(i32 j = 0; j < (i32)pick_cards->size; ++j) {
			cards_id[j] = *(i32*)node_advance(LIST_BEGIN(pick_cards), j)->data;
			sprintf(cards_opt[j], "%2d) 挑選一張牌: %s", j+1, cards[cards_id[j]].name);
		}

		sSelectEvent event = select_event_with_arr(pGame, *(i32 *)cur_p->data , 1, 1, cards_opt, last_size, sizeof(*cards_opt) );
		i32 take_idx = *(i32*)LIST_FRONT(event.select_res);
		i32 take_id = cards_id[take_idx];
		give_card(pGame, pGame->players[cur_p_id].cards, take_id, true);
		take_card_by_id(pGame, pick_cards, take_id);

		cur_p = get_next_player(pGame, cur_p);
	}
	printf(YLW"-> 雜貨店處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_panic(sGame *pGame, i32 player_id, i32 card_id){
	if( player_id == 0 ){
		printf(YLW"-> 你 使用了 驚慌!\n"RST);
		printf(YLW"-> 請選擇你要使用驚慌的目標\n"RST);
	} else {
		printf(YLW"-> player %d 使用了 驚慌!\n"RST,player_id);
		printf(YLW"-> Player %d 正在選擇他的目標\n"RST, player_id);
	}

	sListNode *cur_player_node = pGame->cur_player;
	sPlayer *cur_player = &pGame->players[player_id];
	sList *live_p = pGame->live_players;
	i32 live_size = live_p->size;

	i32 opt_num = 0;
	i32 players_id[live_size];
	char players_option[live_size][512];

	LIST_FOR_EACH(pNode, pGame->live_players) {
		i32 target_id = *(i32*)pNode->data;
		sPlayer *target_player = &pGame->players[target_id];

		sList *target_desk = target_player->desk;
		sList *target_hand = target_player->cards;
		i32 target_cards_cnt = target_desk->size + target_hand->size;

		if(target_id == player_id || target_cards_cnt == 0) continue;

		i32 dis = node_distance( live_p, pNode, cur_player_node );
		dis = min(dis, live_size-dis)
			  - cur_player->look_range
			  + target_player->be_looked_range;
		
		if(contains_card_type(cur_player->desk, SCOPE)) --dis;
		if(contains_card_type(target_player->desk, MUSTANG)) ++dis;
		
		if( dis <= 1 ) {
			players_id[opt_num] = target_id;
			sprintf( players_option[opt_num], "%2d) Player %d", opt_num+1, target_id);
			++opt_num;
		}
	}

	if(opt_num == 0) {
		if( player_id == 0) {
			printf(YLW"-> 你發現沒有可以使用驚慌的目標QQ\n"RST);
		} else {
			printf(YLW"-> Player %d 發現沒有可以使用驚慌的目標OAO\n"RST, player_id);
		}
		return;
	}

	sSelectEvent panic_event = select_event_with_arr(pGame, player_id, 1, 1, players_option, opt_num, sizeof(*players_option));
	i32 target_idx = *(i32*)LIST_FRONT(panic_event.select_res);
	i32 target_id = players_id[target_idx];
	free_list(panic_event.selections);
	free_list(panic_event.select_res);

	if( target_id == 0 ){
		printf(YLW"-> 你被驚慌了QQ\n"RST);
	} else printf(YLW"-> player %d 被驚慌了\n"RST, target_id );

	// build selection

	sPlayer *target = &pGame->players[target_id];
	sList *target_hand = target->cards;
	sList *target_desk = target->desk;
	i32 hand_card_num  = target_hand->size;
	i32 desk_card_num  = target_desk->size;
	i32 total_card_num = hand_card_num + desk_card_num;
	i32 select_card_id = 0;

	char cards_opt[total_card_num][512];
	for(int i = 0; i < total_card_num; ++i ) {
		if(i < hand_card_num) {
			sprintf(cards_opt[i], "%2d) 手牌 %d", i+1, i+1);
		} else {
			i32 id = *(i32*)node_advance(LIST_BEGIN(target_desk), i-hand_card_num)->data;
			sprintf(cards_opt[i], "%2d) 桌面: %s", i+1, cards[id].name);
		}

	}
	sSelectEvent card_select_event = select_event_with_arr(pGame, player_id, 1, 1, cards_opt, total_card_num, sizeof(*cards_opt));
	i32 take_idx = *(i32*)LIST_FRONT(card_select_event.select_res);
	free_list(card_select_event.selections);
	free_list(card_select_event.select_res);
		
	if(take_idx < hand_card_num){
		select_card_id = take_card(pGame, target_hand, take_idx);
	}else{
		select_card_id = take_card(pGame, target_desk, take_idx - hand_card_num);
	}
		
	if( select_card_id != -1) {
		give_card(pGame, cur_player->cards, select_card_id, true);
	}

	printf(YLW"-> 驚慌處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_stagecoach(sGame *pGame, i32 player_id, i32 card_id ) {
	if( player_id == 0 ){
		printf(YLW"-> 你使用了驛馬車\n"RST);
	}else printf(YLW"-> player %d 使用了驛馬車\n"RST,player_id);

	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_array_id;
	for(i32 i = 0; i < 2; ++i) {
		card_array_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_array_id, true);
	}

	printf(YLW"-> 驛馬車處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_wells_fargo(sGame *pGame, i32 player_id, i32 card_id ) {
	if( player_id == 0 ){
		printf(YLW"-> 你使用了威爾斯法歌\n"RST);
	}else printf(YLW"-> player %d 使用了威爾斯法歌\n"RST,player_id);

	sListNode *cur_p = get_player(pGame, player_id);
	i32 card_array_id;
	for(i32 i = 0; i < 3; ++i) {
		card_array_id = take_card(pGame, pGame->draw_pile, 0);
		give_card(pGame, pGame->players[*(i32*)cur_p->data].cards, card_array_id, true);
	}
	printf(YLW"-> 威爾斯法歌處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_beer(sGame *pGame, i32 player_id, i32 card_id ) {
	if( (i32)pGame->live_players->size >= 2  ) {
		if( player_id == 0 ){
			printf(YLW"-> 你使用了啤酒\n"RST);
		}else printf(YLW"-> player %d 使用了 啤酒\n"RST,player_id);

		i32 id = *(i32 *)pGame->cur_player->data;
		i32 character_id = pGame->players[player_id].character;
		i32 highest_hp = characters[ character_id ].hp;

		if( pGame->players[id].hp<highest_hp ){
			pGame->players[id].hp += 1;
		}
	}

	printf(YLW"->啤酒處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_cat_balou(sGame *pGame, i32 player_id, i32 card_id ) {
	if( player_id == 0 ){
		printf(YLW"-> 您將使用凱特巴洛!\n"RST);
		printf(YLW"-> 請選擇一位玩家，其將自行選擇棄掉任何一張牌。\n"RST);
	}else printf(YLW"-> player %d 將使用凱特巴洛\n"RST,player_id);

	//選人，除了自己。
	i32 live_size = pGame->live_players->size;
	i32 players_id[live_size];
	char players_option[live_size][512];

	int cnt=0;
	LIST_FOR_EACH(pNode, pGame->live_players) {
		i32 target_id = *(i32 *)pNode->data;
		sList *target_desk = pGame->players[ target_id ].desk;
		sList *target_hand = pGame->players[ target_id ].cards;
		i32 target_cards_cnt = target_desk->size + target_hand->size;

		if(target_id == player_id || target_cards_cnt == 0) continue;

		players_id[cnt] = target_id;
		sprintf(players_option[cnt], "%d) player%d\n", cnt+1, target_id);
		++cnt;
	}
	
	sSelectEvent sl_e = select_event_with_arr(pGame, player_id, 1, 1, players_option, cnt, sizeof(*players_option));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	i32 target_id =  players_id[select_idx];
	free_list(sl_e.select_res);
	free_list(sl_e.selections);

	if( target_id == 0 ){
		printf(YLW"-> 你被選擇了\n"RST);
	}else printf(YLW"-> player %d 被選擇了\n"RST, target_id);

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

	printf(YLW"-> player %d 棄掉了 %s\n"RST, target_id, cards[ id ].name);

	printf(YLW"-> 凱特巴洛處理完畢\n"RST);
	take_card_by_id( pGame, pGame->players[player_id].cards , card_id );
	give_card( pGame, pGame->discard_pile , card_id , true );
}

void card_jail(sGame *pGame, i32 player_id, i32 card_id){
	if( player_id == 0 ){
		printf(YLW"-> 你使用了監獄\n"RST);
		printf(YLW"-> 請選擇使用對象:\n"RST);
	}else printf(YLW"-> player %d 使用了監獄\n"RST,player_id);
	i32 select_idx = select_player( pGame, player_id );
	
	
	if( select_idx == 0 ){
		printf(YLW"-> 你被關進監獄了QQ\n"RST);
	}else printf(YLW"-> player %d 被關進監獄了QQ\n"RST,select_idx);
	// 把jail放到某個人的桌上
	// 把jail從自己手牌移除
	printf(YLW"-> 監獄處理完畢\n"RST);
	give_card( pGame, pGame->players[select_idx].desk, card_id, true );
	take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_bomb(sGame *pGame, i32 player_id, i32 card_id){
	// 把bomb從手牌移走 是外面要做的事情
	if( player_id == 0 ){
		printf(YLW"-> 你放了一個炸彈\n"RST);
	}else printf(YLW"-> player %d 放了一個炸彈\n"RST,player_id);

	printf(YLW"-> 炸彈放置完畢\n"RST);
	give_card( pGame, pGame->players[player_id].desk, card_id, true );
	take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_gatling(sGame *pGame, i32 player_id, i32 card_id ){
    // 從自己開始繞一圈，問每個人要不要丟miss，不要->扣血。 
    // 直接丟到棄牌堆
	if( player_id == 0 ){
		printf(YLW"-> 你使用了格林機關槍\n"RST);
	}else printf(YLW"-> player %d 使用了格林機關槍\n"RST,player_id);

    LIST_FOR_EACH(pNode, pGame->live_players) {
        i32 id = *(i32 *)pNode->data;
        if(id  != player_id) {
            i32 id_choose=select_throw( pGame, id, MISS );
            if( id_choose == -1 ){
                //不丟bang就扣血
                damage_event( pGame, id, player_id, 1 );
		
				if(pGame->players[id].hp<=0){
					sLethalEvent lth_e = lethal_event(pGame, id);
					if(lth_e.lethal_res == true) {
						sDeathEvent dth_e = death_event(pGame, id, player_id);
						// cur_player died
						if(dth_e.death_res == true) {
							if( id == 0 ){
								printf(YLW"-> You died\n"RST);
							}else printf(YLW"-> player %d died"RST,id);
							return;
						}
					}
				}

                if( id==0 ){
                    printf(YLW"-> 你被扣一滴血了QQ\n"RST);
                }else printf(YLW"-> player %d 被扣了一滴血\n"RST,id);

                break;
            }

            take_card_by_id( pGame, pGame->players[id].cards, id_choose );
            give_card( pGame, pGame->discard_pile, id_choose, true );
        }
    }

	printf(YLW"-> 格林機關槍使用完畢\n"RST);
    give_card( pGame, pGame->discard_pile, card_id, false);
    take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_indians(sGame *pGame, i32 player_id, i32 card_id ){
    // 從自己開始繞一圈，問每個人要不要丟bang，不要->扣血。
    // 直接丟到棄牌堆
	if( player_id == 0 ){
		printf(YLW"-> 你使用了印地安人\n"RST);
	}else printf(YLW"-> player %d 使用了印地安人\n"RST,player_id);

    LIST_FOR_EACH(pNode, pGame->live_players) {
        i32 id = *(i32 *)pNode->data;
        if(id  != player_id) {
            i32 id_choose=select_throw( pGame, id, BANG );
            if( id_choose == -1 ){
                //不丟bang就扣血
                damage_event( pGame, id, player_id, 1 );
				if(pGame->players[id].hp<=0){
					sLethalEvent lth_e = lethal_event(pGame, id);
					if(lth_e.lethal_res == true) {
						sDeathEvent dth_e = death_event(pGame, id, player_id);
						// cur_player died
						if(dth_e.death_res == true) {
							if( id == 0 ){
								printf(YLW"-> You died\n"RST);
							}else printf(YLW"-> player %d died\n"RST,id);
							return;
						}
					}
				}
                if( id==0 ){
                    printf(YLW"-> 你被扣一滴血了QQ\n"RST);
                }else printf(YLW"-> player %d 被扣了一滴血\n"RST,id);
                break;
            }else{
				if( id==0 ){
					printf(YLW"-> 你丟出了一張Bang以避免扣血\n"RST);
				}else printf(YLW"-> player %d 丟出了一張Bang以避免扣血\n"RST,id);
			}
			
            take_card_by_id( pGame, pGame->players[id].cards, id_choose );
            give_card( pGame, pGame->discard_pile, id_choose, true );
        }
    }

	printf(YLW"-> 印地安人使用完畢\n"RST);
    give_card( pGame, pGame->discard_pile, card_id, true );
    take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
}

void card_duel(sGame *pGame, i32 player_id, i32 card_id ){
	// 用while()，跑你、我、你、我，直到偵測到throw_card回傳-1，就break。 
	// 直接丟到棄牌堆
	if( player_id == 0 ){
		printf(YLW"-> 你丟出了決鬥\n"RST);
		printf(YLW"-> 請選擇決鬥對象：\n"RST);
	}else printf(YLW"-> player %d 丟出了 決鬥\n"RST,player_id);

	i32 duel_id=select_player(pGame, player_id);
	if ( duel_id==0 ){
		printf(YLW"-> 你成為 player %d 的決鬥對象\n"RST,player_id);
	}else printf(YLW"-> player %d 成為 player %d 的決鬥對象\n"RST,duel_id,player_id);
	
	while(1){
		if( duel_id == 0 ) printf(YLW"-> 您需要丟出一張bang,否則將會扣一滴血\n"RST);
		i32 duel_id_choose=select_throw( pGame, duel_id, BANG );

		if( duel_id_choose == -1 ){
			//不丟bang就扣血
			//pGame->players[duel_id].hp--;阿姨不給機會:)
			damage_event( pGame, duel_id, player_id, 1 );
			if(pGame->players[duel_id].hp<=0){
				sLethalEvent lth_e = lethal_event(pGame, duel_id);
				if(lth_e.lethal_res == true) {
					sDeathEvent dth_e = death_event(pGame, duel_id, player_id);
					// cur_player died
					if(dth_e.death_res == true) {
						if( duel_id == 0 ){
							printf(YLW"-> You died\n"RST);
						}else printf(YLW"-> player %d died\n"YLW,duel_id);
						return;
					}
				}
			}
			if( duel_id==0 ){
				printf(YLW"-> 你被扣一滴血了QQ\n"RST);
			}else printf(YLW"-> player %d 被扣了一滴血\n"RST,duel_id);
			break;
		}
		take_card_by_id( pGame, pGame->players[duel_id].cards, duel_id_choose );
		give_card( pGame, pGame->discard_pile, duel_id_choose, true );

		if( player_id == 0 ) printf(YLW"-> 您需要丟出一張bang,否則將會扣一滴血\n"RST);

		i32 player_id_choose=select_throw( pGame, player_id, BANG );
		if( player_id_choose == -1 ){
			//不丟bang就扣血
			//pGame->players[player_id].hp--;阿姨不給機會:)
			damage_event( pGame, player_id, duel_id, 1 );
			if(pGame->players[player_id].hp<=0){
				sLethalEvent lth_e = lethal_event(pGame, player_id);
				if(lth_e.lethal_res == true) {
					sDeathEvent dth_e = death_event(pGame, player_id, duel_id);
					// cur_player died
					if(dth_e.death_res == true) {
						if( player_id == 0 ){
							printf("You died\n");
						}else printf("> player %d died",player_id);
					}
				}
			}

			if( player_id==0 ){
				printf(YLW"-> 你被扣一滴血了QAQ\n"RST);
			}else printf(YLW"-> player %d 被扣了一滴血\n"RST,player_id);
			break;
		}
		take_card_by_id( pGame, pGame->players[player_id].cards, player_id_choose );
		give_card( pGame, pGame->discard_pile, player_id_choose, true );
	}

	printf(YLW"-> 決鬥使用完畢\n"RST);
	give_card( pGame, pGame->discard_pile, card_id, true );
    take_card_by_id( pGame, pGame->players[player_id].cards, card_id );
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