#include "game_play.h"


//須拿到花色才能接著做
void prep_phase(sGame *pGame) { 
	// 卡牌存： pGame->players[*(i32 *)pGame->cur_player->data ]->desk ;
	// Determine bomb(79) ,and then determine jail(74.75.76).
	i32 flag_bomb=0;
	i32 flag_prison=0;
	LIST_FOR_EACH(pNode,pGame->players[*(i32 *)pGame->cur_player->data ].desk){
		if(pNode->data==79){
			flag_bomb=1;
			break;
		}else if(pNode->data>=74 && pNode->data<=76){
			flag_prison=1;
			break;
		}
	}

	if( flag_bomb ){
		i32 fortune=(i32)draw(pGame->draw_pile);

		//fortune若為黑桃的2~9，炸開且扣3點生命；否則傳給下一個人。
		//...
	}
	if( flag_prison ){
		i32 fortune=(i32)draw(pGame->draw_pile);

		//fortune若為紅心則逃獄成功
	}
}

void draw_phase(sGame *pGame) {  
	// draw two cards
	// put in "draw.h"
	draw_card( pGame, pGame->players[*(i32 *)pGame->cur_player->data ].id , 2 );
}

void play_phase(sGame *pGame) {  // play any number of cards

}

void discard_phase(sGame *pGame) {  
	// discard excess cards
	// put in "discard.h"
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