#include "card_func.h"


void card_miss(sGame *pGame, i32 player_id) {

}

void card_bang(sGame *pGame, i32 player_id) {

}

void (*card_funcs[80])(sGame *pGame, i32 player_id) = {
	&card_miss,
	&card_bang,
};