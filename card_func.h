#pragma once

#include "game_play.h"
#include "draw.h"
#include "event.h"

extern void (*card_funcs[80])(sGame *pGame, i32 player_id);

void init_card_funcs();
// cards
// void card_miss(sGame *pGame, i32 player_id);
// void card_bang(sGame *pGame, i32 player_id);

void card_beer(sGame *pGame, i32 player_id);
void card_saldon(sGame *pGame, i32 player_id);
void card_stagecoach(sGame *pGame, i32 player_id);
void card_wells_fargo(sGame *pGame, i32 player_id);

void card_general_store(sGame *pGame, i32 player_id);
// void card_panic(sGame *pGame, i32 player_id);
void card_cat_balou(sGame *pGame, i32 player_id);

// void card_gatlin(sGame *pGame, i32 player_id);         // 從自己開始繞一圈，問每個人要不要丟miss，不要->扣血。
// void card_indians(sGame *pGame, i32 player_id);        // 從自己開始繞一圈，問每個人要不要丟bang，不要->扣血。
// void card_duel(sGame *pGame, i32 player_id);           // 用while()，跑你、我、你、我，直到偵測到throw_card回傳1，就break。
int throw_card(sGame *pGame, i32 player_id, i32 card_id); // 叫該player丟該card，如果他沒辦法做到的話，就得扣血，也就是回傳1，否則回傳0。

// table
// void card_mustang(sGame *pGame, i32 player_id);
// void card_scope(sGame *pGame, i32 player_id);
// void card_barrel(sGame *pGame, i32 player_id);
// void card_jail(sGame *pGame, i32 player_id);
// void card_bomb(sGame *pGame, i32 player_id);

// guns
// void remington(sGame *pGame, i32 player_id);
// void schofild(sGame *pGame, i32 player_id);
// void winchester(sGame *pGame, i32 player_id);
// void carabine(sGame *pGame, i32 player_id);
// void volcano(sGame *pGame, i32 player_id);