#include "game_play.h"
#include "display.h"
#include "select.h"
#include "play_phase.h"
#include "display.h"

bool contains_card_type(sList *pPile, eCardType card_type) {
	LIST_FOR_EACH(pNode, pPile) {
		i32 card_id = *(i32*)pNode->data;
		if(cards[card_id].type == card_type) return true;
	}
	return false;
}

bool is_bomb(i32 card_id) {
	return card_id == 79;
}

bool is_jail(i32 card_id) {
	return card_id >= 74 && card_id <= 76;
}

sListNode *get_player(sGame *pGame, i32 id) {
	sListNode *res = NULL;
	LIST_FOR_EACH(pNode, pGame->live_players) {
		if(*(i32*)pNode->data == id) {
			res = pNode;
			break;
		}
	}
	return res;
}

void remove_live_player(sGame *pGame, i32 id) {
	sListNode *pPlayerNode = get_player(pGame, id);
	if(pGame->cur_player == pPlayerNode) pGame->cur_player = get_next_player(pGame, pPlayerNode);
	list_erase(pGame->live_players, pPlayerNode);

}

sListNode *get_prev_player(sGame *pGame, sListNode *pPlayerNode) {
	sListNode *prev_player = pPlayerNode->prev;
	return (prev_player != LIST_END(pGame->live_players)) ?
				prev_player : prev_player->prev;
}

sListNode *get_next_player(sGame *pGame, sListNode *pPlayerNode) {
	sListNode *next_player = pPlayerNode->next;
	return (next_player != LIST_END(pGame->live_players)) ?
				next_player : next_player->next;
}

sList *card_filter(sList *cards, bool (*filter)(i32 card_id)) {
	sList *res = new_list();
	LIST_FOR_EACH(pNode, cards) {
		i32 card_id = *(i32*)pNode->data;
		if(filter(card_id) == false) continue;
		i32 *data = malloc(sizeof(i32));
		*data = card_id;
		list_push_back(res, new_node(data));
	}
	return res;
}

//須拿到花色才能接著做
i32 prep_phase(sGame *pGame) {
	// 卡牌存： pGame->players[*(i32 *)pGame->cur_player->data ]->desk ;
	// Determine bomb(79) ,and then determine jail(74.75.76).
	i32 bomb_id = -1, jail_id = -1;

	i32 cur_player_id = *(i32*)pGame->cur_player->data;
	i32 next_player_id = *(i32*)get_next_player(pGame, pGame->cur_player)->data;
	sPlayer *cur_player = &pGame->players[cur_player_id];
	sPlayer *next_player = &pGame->players[next_player_id];
	LIST_FOR_EACH(pNode, cur_player->desk){
		i32 card_id = *(i32*)(pNode->data);
		if(is_bomb(card_id)){
			bomb_id = card_id;
			continue;
		}
		if(is_jail(card_id)) {
			jail_id = card_id;
			continue;
		}
	}

	if(bomb_id != -1) {
		display_has_bomb(pGame, 0, cur_player_id);
		take_card_by_id(pGame, cur_player->desk, bomb_id);
		sDetermineEvent dtm_e = determine_event(pGame, cur_player_id);
		i32 dtm_res = dtm_e.determine_res;
		i32 num = cards[dtm_res].num;
		eSuit suit = cards[dtm_res].suit;
		if(suit == SPADE && (num >=2 || num <= 9)) {
			//若為黑桃的2~9，炸開且扣3點生命。
			damage_event(pGame, cur_player_id, -1, 3);  // 炸彈的 damager_id = -1
			give_card(pGame,pGame->discard_pile, bomb_id, false);
			if(cur_player->hp <= 0) {
				sLethalEvent lth_e = lethal_event(pGame, cur_player_id);
				if(lth_e.lethal_res == true) {
					sDeathEvent dth_e = death_event(pGame, cur_player_id, -1);
					// cur_player died
					if(dth_e.death_res == true) {
						if( cur_player_id == 0 ){
							printf("You died by bomb\n");
						}else printf("> player %d died by bomb\n",cur_player_id );
						return -1;
					}
				}
			}
		}
		else {
			//傳給下一個人。
			give_card(pGame, next_player->desk, bomb_id, true);
		}
	}
	if(jail_id != -1){
		//如果逃獄成功，回傳0，失敗則-1。
		sDetermineEvent dtm_e = determine_event(pGame, cur_player_id);
		i32 dtm_res = dtm_e.determine_res;
		eSuit suit = cards[dtm_res].suit;
		if(suit != HEART){
			// 非紅心，逃獄失敗
			return -1;
		}
	}
	
	return 0;
}

void draw_phase(sGame *pGame) {
	// draw two cards
	// put in "draw.h"
	i32 cur_player_id = *(i32*)pGame->cur_player->data;
	draw_phase_event(pGame, cur_player_id);
}

void play_phase(sGame *pGame) {  // play any number of cards
	i32 cur_player_id = *(i32*)pGame->cur_player->data;
	play_phase_default(pGame, cur_player_id);
}

void discard_phase(sGame *pGame) {
	// discard excess cards
	// put in "discard.h"
	i32 cur_player_id = *(i32*)pGame->cur_player->data;
	discard(pGame, cur_player_id);
}

void turn_phase(sGame *pGame) {
	// if(pGame->live_players->size == 0) exit(1);

	// death_event(pGame, *(i32*)LIST_FRONT(pGame->live_players), -1);

	// return;

	char appellation[16];
	i32 skip = false;
	i32 cur_player_id = *(i32*)pGame->cur_player->data;

	display_board(pGame, 0);
	
	if(cur_player_id == 0) sprintf(appellation, "  You  ");
	else                   sprintf(appellation, "Player%1d", cur_player_id);
	
	printf(RED"準備階段 ( %s ):\n"RST, appellation);
	skip = (prep_phase(pGame) == -1);
	if(skip) return;
	printf("\n");

	display_hands(pGame, 0);

	printf(RED"抽牌階段 ( %s ):\n"RST, appellation);
	draw_phase(pGame);
	printf("\n");

	display_hands(pGame, 0);

	printf(RED"出牌階段 ( %s ):\n"RST, appellation);
	play_phase(pGame);
	if(pGame->end_winner_role != (eRole)-1) return;
	printf("\n");

	display_hands(pGame, 0);

	printf(RED"棄牌階段 ( %s ):\n"RST, appellation);
	discard_phase(pGame);
	printf("\n");

	pGame->cur_player = get_next_player( pGame, pGame->cur_player );
}

void game_end(sGame *pGame) {
	display_end(pGame, pGame->end_winner_role);
}


void play_game(i32 num_players) {
	sGame *pGame = new_game();  // malloc
	init_game(pGame, num_players);  // set up pGame
	while(pGame->end_winner_role == (eRole)-1) {
		turn_phase(pGame);  // process pGame
	}
	game_end(pGame);
	free_game(pGame);
}