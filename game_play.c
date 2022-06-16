#include "game_play.h"

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
	bool flag_bomb=false;
	bool flag_prison=false;
	sListNode* pNode;
	sDetermineEvent result;
	i32 take_bumb=0;
	sListNode* next_player;
	if( pGame->cur_player->next == pGame->live_players->end){
		next_player=LIST_BEGIN(pGame->live_players);
	}else next_player=pGame->cur_player->next;

	LIST_FOR_EACH(pNode,pGame->players[*(i32 *)pGame->cur_player->data ].desk){
		if(*(i32*)(pNode->data)==79){
			take_bumb=take_card_by_id( pGame,pGame->players[ *(i32*)pGame->cur_player->data ].desk,79 );
			flag_bomb=true;
			break;
		}else if(*(i32*)(pNode->data)>=74 && *(i32*)(pNode->data)<=76){
			flag_prison=true;
			break;
		}
	}

	if( flag_bomb ){
		result=determine_event( pGame,*(i32 *)pGame->cur_player->data );
		if( cards[result.determine_res].suit==SPADE && ( cards[ result.determine_res].num>=2 || cards[ result.determine_res].num<=9 ) ){
			//若為黑桃的2~9，炸開且扣3點生命。
			damage_event( pGame, *(i32*)pGame->cur_player->data,-1, 3 );
			give_card(pGame,pGame->discard_pile,take_bumb,false);
		}else{
			//傳給下一個人。
			give_card(pGame,pGame->players[ *(i32*)(next_player->data) ].desk,take_bumb,false);
		}
	}
	if( flag_prison ){
		//如果逃獄成功，回傳0，失敗則-1。
		result=determine_event( pGame,*(i32 *)pGame->cur_player->data );
		if( cards[result.determine_res].suit==HEART ){
			//為紅心則逃獄成功
		}else return -1;

	}
	
	free(pNode);
	free(next_player);
	return 0;
}

void draw_phase(sGame *pGame) {  
	// draw two cards
	// put in "draw.h"
	i32 player_id = *(i32 *)pGame->cur_player->data;
	draw_phase_event(pGame, *(i32 *)pGame->cur_player->data);
}

void play_phase(sGame *pGame) {  // play any number of cards

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