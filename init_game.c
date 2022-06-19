#include "init_game.h"

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
		pGame->players[i].id = i;
		pGame->players[i].attack_range = 1;
		pGame->players[i].role = *(eRole*)LIST_FRONT(pGame->role_pile);
		list_pop_front(pGame->role_pile);
		pGame->players[i].character = *(i32*)LIST_FRONT(pGame->character_pile);
		list_pop_front(pGame->character_pile);

		pGame->players[i].cards = new_list();
		pGame->players[i].desk  = new_list();

		// TODO setup hp
		// register event_func by character
		pGame->players[i].hp = 2; //  characters[pGame->players[i].character].hp;
		
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