#include "play_phase.h"
#include "game_play.h"
#include "card_func.h"

void play_phase_default(sGame *pGame, i32 player_id) {
	static const int WILLY = 0;
	static const int JANET = 3;
	sPlayer *pPlayer = &pGame->players[player_id];
	sList *hands = pPlayer->cards;
	sList *desk = pPlayer->desk;
	i32 hands_size = hands->size;

	// init bang times
	i32 bang_times = 0;
	i32 max_bang_times = 1;
	if(pPlayer->character == WILLY
		|| contains_card_type(desk, VOLCANIC) ) {
			max_bang_times = INT_MAX;
	}
	

	while(true) {
		i32 cnt = 0;
		i32 hands_id[hands_size];
		char options[hands_size][16];
		LIST_FOR_EACH(pNode, hands) {
			i32 card_id = *(i32*)pNode->data;
			sCard *card = &cards[card_id];

			if(bang_times < max_bang_times) {  // can bang
				if(card->type == MISS && pPlayer->character != JANET) continue;
			}
			else {
				if(card->type == MISS || card->type == BANG) continue;
			}
			hands_id[cnt] = card_id;
			sprintf(options[cnt], "%2d) %s", cnt+1, card->name);
			++cnt;
		}
		if(cnt == 0) break;
		
		sSelectEvent sl_e = select_event_with_arr(pGame, player_id, 1, 1, options, cnt, sizeof(*options));
		i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
		free_list(sl_e.select_res);
		free_list(sl_e.selections);
		i32 select_card_id = hands_id[select_idx];
		switch(cards[select_card_id].type) {
		case MISS:
		case BANG:
			++bang_times;
			card_bang(pGame, player_id, select_card_id);
			break;
		case INDIANS:
			card_bang(pGame, player_id, select_card_id);
			break;
		case DUAL:
			// card_dual(pGame, player_id, select_card_id);
			break;
		case SALOON:
			card_saldon(pGame, player_id, select_card_id);
			break;
		case GENERAL_STORE:
			card_general_store(pGame, player_id, select_card_id);
			break;
		case STAGECOACH:
			card_stagecoach(pGame, player_id, select_card_id);
			break;
		case WELLS_FARGO:
			card_wells_fargo(pGame, player_id, select_card_id);
			break;
		case PANIC:
			// card_panic(pGame, player_id, select_card_id);
			break;
		case GATLING:
			// card_gatling(pGame, player_id, select_card_id);
			break;
		case CAT_BALOU:
			card_cat_balou(pGame, player_id, select_card_id);
			break;
		case BEER:
			card_beer(pGame, player_id, select_card_id);
			break;
		case REMINGTON:
			card_remington(pGame, player_id, select_card_id);
			break;
		case SCHOFIELD:
			card_schofild(pGame, player_id, select_card_id);
			break;
		case WINCHESTER:
			card_winchester(pGame, player_id, select_card_id);
			break;
		case CARABINE:
			card_carabine(pGame, player_id, select_card_id);
			break;
		case VOLCANIC:
			card_volcano(pGame, player_id, select_card_id);
			break;
		case SCOPE:
			card_scope(pGame, player_id, select_card_id);
			break;
		case MUSTANG:
			card_mustang(pGame, player_id, select_card_id);
			break;
		case JAIL:
			card_jail(pGame, player_id, select_card_id);
			break;
		case BARREL:
			card_barrel(pGame, player_id, select_card_id);
			break;
		case DYNAMITE:
			// card_dynamite(pGame, player_id, select_card_id);
			break;
		}
		if(pGame->end_winner_role != (eRole)-1) break;
	}
}