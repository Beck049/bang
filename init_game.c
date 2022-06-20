#include "init_game.h"

#include "display.h"
#include "event.h"
#include "bang.h"
#include "damage.h"
#include "death.h"
#include "determine.h"
#include "dodge.h"
#include "draw.h"
#include "lethal.h"
#include "select.h"

// init draw pile and shuffle
void init_draw_pile(sGame *pGame) {
	list_init(pGame->draw_pile, card_num);
	shuffle(pGame->draw_pile);
}

// init role pile and shuffle
void init_role_pile(sGame *pGame) {
	for(i32 i = 0; i < ROLE_SIZ; ++i) {  // loop role
		// push role into list
		for(i32 j = 0; j < ROLE_DIVIDED[pGame->total_players][i]; ++j) {
			eRole *role = malloc(sizeof(eRole));
			*role = i;
			list_push_back(pGame->role_pile, new_node(role));
		}
	}
	shuffle(pGame->role_pile);
}

// init character pile and shuffle
void init_character_pile(sGame *pGame) {
	list_init(pGame->character_pile, character_num);
	shuffle(pGame->character_pile);
}

// init live players
void live_players_init(sGame *pGame) {
	list_init(pGame->live_players, pGame->total_players);
	shuffle(pGame->live_players);
}

// set up player info
void setup_players(sGame *pGame) {
	for(i32 i = 0; i < pGame->total_players; ++i) {
		register_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_default);
		register_event_func(EVENT_PLAY_PHASE, i, play_phase);
		register_event_func(EVENT_DAMAGE, i, damage_event_default);
		register_event_func(EVENT_DETERMINE, i, determine_event_default);
		register_event_func(EVENT_DODGE, i, dodge_event_default);
		register_event_func(EVENT_BANG, i, bang_event_default);
		register_event_func(EVENT_DEATH, i, death_event_default);
		register_event_func(EVENT_LETHAL, i, lethal_event_default);

		if(i == 0) register_event_func(EVENT_SELECT, i, select_event_player);
		else register_event_func(EVENT_SELECT, i, select_event_bot);
	}

	for(i32 i = 0; i < pGame->total_players; ++i) {
		sPlayer *pPlayer = &pGame->players[i];
		pPlayer->id = i;
		pPlayer->attack_range = 1;
		pPlayer->look_range = 0;
		pPlayer->be_looked_range = 0;
		pPlayer->role = *(eRole*)LIST_FRONT(pGame->role_pile);
		list_pop_front(pGame->role_pile);
		pPlayer->character = *(i32*)LIST_FRONT(pGame->character_pile);
		list_pop_front(pGame->character_pile);

		pPlayer->cards = new_list();
		pPlayer->desk  = new_list();

		// TODO setup hp
		// register event_func by character
		pPlayer->hp = characters[pPlayer->character].hp;

		printf("Player%d:\n", i);
		printf("hp: %d\n", pPlayer->hp);
		printf("role: %d\n", pPlayer->role);
		printf("char: %d\n", pPlayer->character);
		printf("l_rng: %d\n", pPlayer->look_range);
		printf("bl_rng: %d\n", pPlayer->be_looked_range);
		printf("===================\n");

		switch(pPlayer->character) {
		case 0:
			break;
		case 1:
			unregister_event_func(EVENT_DAMAGE, i, damage_event_default);
			register_event_func(EVENT_DAMAGE, i, damage_event_bart_cassidy);
			break;
		case 2:
			unregister_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_default);
			register_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_black_jack);
			break;
		case 3:
			unregister_event_func(EVENT_DODGE, i, dodge_event_default);
			register_event_func(EVENT_DODGE, i, dodge_event_calamity_janet);
			break;
		case 4:
			unregister_event_func(EVENT_DAMAGE, i, damage_event_default);
			register_event_func(EVENT_DAMAGE, i, damage_event_el_gringo);
			break;
		case 5:
			unregister_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_default);
			register_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_jesse_jones);
			break;
		case 6:
			unregister_event_func(EVENT_DODGE, i, dodge_event_default);
			register_event_func(EVENT_DODGE, i, dodge_event_barrel);
			break;
		case 7:
			unregister_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_default);
			register_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_kit_carlson);
			break;
		case 8:
			unregister_event_func(EVENT_DETERMINE, i, determine_event_default);
			register_event_func(EVENT_DETERMINE, i, determine_event_lucky_duke);
			break;
		case 9:
			pPlayer->be_looked_range += 1;
			break;
		case 10:
			unregister_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_default);
			register_event_func(EVENT_DRAW_PHASE, i, draw_phase_event_pedro_ramirez);
			break;
		case 11:
			pPlayer->look_range += 1;
			break;
		case 12:
			unregister_event_func(EVENT_LETHAL, i, lethal_event_default);
			register_event_func(EVENT_LETHAL, i, lethal_event_sid_ketchum);
			break;
		case 13:
			unregister_event_func(EVENT_BANG, i, bang_event_default);
			register_event_func(EVENT_BANG , i, bang_event_slab_the_killer);
			break;
		case 14:
			break;
		case 15:
			for(i32 j = 0; j < pGame->total_players; ++j) {
				if(j == i) continue;
				unregister_event_func(EVENT_DEATH, j, death_event_default);
				register_event_func(EVENT_DEATH, j, death_event_vulture_sam);
			}
			break;
		}

		for(int j = 0; j < pGame->players[i].hp; ++j) {
			i32 card_id = take_card(pGame, pGame->draw_pile, 0);
			printf("!!%d\n", card_id);
			give_card(pGame, pGame->players[i].cards, card_id, true);
		}
	}
}

// init game
void init_game(sGame *pGame, i32 num_players) {
	pGame->end_winner_role = -1;
	pGame->total_players = num_players;
	init_cards();
	init_characters();
	init_event_funcs();
	init_draw_pile(pGame);
	init_role_pile(pGame);
	display_pile(pGame->draw_pile);
	init_character_pile(pGame);
	live_players_init(pGame);
	setup_players(pGame);
	pGame->cur_player = LIST_BEGIN(pGame->live_players);
}

void cleanup_game(sGame *pGame) {
	list_clear(pGame->draw_pile);
	list_clear(pGame->discard_pile);
	list_clear(pGame->role_pile);
	list_clear(pGame->character_pile);
	list_clear(pGame->live_players);
}