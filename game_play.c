#include "game_play.h"

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
	i32 next_player_id = *(i32*)get_next_player(pGame, pGame->cur_player);
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
		sDetermineEvent dtm_e = determine_event(pGame, cur_player_id);
		i32 dtm_res = dtm_e.determine_res;
		i32 num = cards[dtm_res].num;
		eSuit suit = cards[dtm_res].suit;
		if(suit == SPADE && (num >=2 || num <= 9)) {
			//若為黑桃的2~9，炸開且扣3點生命。
			damage_event(pGame, cur_player_id, -1, 3);
			take_card_by_id(pGame, cur_player->desk, bomb_id);
			give_card(pGame,pGame->discard_pile, bomb_id, false);
			if(cur_player->hp <= 0) {
				sLethalEvent lth_e = lethal_event(pGame, cur_player_id);
				if(lth_e.lethal_res == true) {
					sDeathEvent dth_e = death_event(pGame, cur_player_id, -1);
					// cur_player died
					if(dth_e.death_res == true) return -1;
				}
			}
		}
		else {
			//傳給下一個人。
			give_card(pGame, next_player->desk, bomb_id, false);
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
	play_phase_event(pGame, cur_player_id);
}

void discard_phase(sGame *pGame) {  
	// discard excess cards
	// put in "discard.h"
	discard( pGame );
}

void turn_phase(sGame *pGame) {
	// print_game(pGame);
	// prep_phase(pGame);
	// draw_phase(pGame);
	// play_phase(pGame);
	// discard_phase(pGame);
}

void game_end(sGame *pGame) {

}


void play_game(i32 num_players) {
	sGame *pGame = new_game();  // malloc
	game_init(pGame, num_players);  // set up pGame
	// while(!pGame->is_end) {
		// turn_phase(pGame);  // process pGame
	// }
	game_end(pGame);
	free_game(pGame);
}