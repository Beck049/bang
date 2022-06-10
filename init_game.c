#include "init_game.h"

// init draw pile and shuffle
void draw_pile_init(sGame *pGame) {
	list_init(pGame->draw_pile, card_num);
	shuffle(pGame->draw_pile);
}

// init role pile and shuffle
void role_pile_init(sGame *pGame) {
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
void character_pile_init(sGame *pGame) {
	list_init(pGame->character_pile, character_num);
	shuffle(pGame->character_pile);
}

// init live players
void live_players_init(sGame *pGame) {
	list_init(pGame->live_players, pGame->total_players);
	shuffle(pGame->live_players);
}

// set up player info
void players_setup(sGame *pGame) {
	for(i32 i = 0; i < pGame->total_players; ++i) {
		pGame->players[i].id = i;
		pGame->players[i].attack_range = 1;
		pGame->players[i].role = *(eRole*)LIST_FRONT(pGame->role_pile);
		list_pop_front(pGame->role_pile);
		pGame->players[i].character = *(i32*)LIST_FRONT(pGame->role_pile);
		list_pop_front(pGame->role_pile);

		// TODO setup hp
		// pGame->players[i].hp = characters[pGame->players[i].character].hp;
		
		draw_event(pGame, pGame->players[i].cards, pGame->players[i].id, pGame->draw_pile, GAME_ID, pGame->players[i].hp, 1);
	}
}

// init game
void game_init(sGame *pGame, i32 num_players) {
	pGame->total_players = num_players;
	draw_pile_init(pGame);
	role_pile_init(pGame);
	character_pile_init(pGame);
	live_players_init(pGame);
	players_setup(pGame);
	pGame->cur_player = LIST_BEGIN(pGame->live_players);
}

void game_cleanup(sGame *pGame) {
	list_clear(pGame->draw_pile);
	list_clear(pGame->discard_pile);
	list_clear(pGame->role_pile);
	list_clear(pGame->character_pile);
	list_clear(pGame->live_players);
}