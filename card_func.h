#pragma once

#include "game_play.h"
#include "draw.h"
#include "event.h"

// cards
void card_miss(sGame *pGame, i32 player_id, i32 card_id );
void card_bang(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆

void card_beer(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆
void card_saldon(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆
void card_stagecoach(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆
void card_wells_fargo(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆

void card_general_store(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆
void card_panic(sGame *pGame, i32 player_id);
void card_cat_balou(sGame *pGame, i32 player_id, i32 card_id );//直接丟到棄牌堆

void card_gatlin(sGame *pGame, i32 player_id, i32 card_id );  // 從自己開始繞一圈，問每個人要不要丟miss，不要->扣血。 //直接丟到棄牌堆
void card_indians(sGame *pGame, i32 player_id, i32 card_id ); // 從自己開始繞一圈，問每個人要不要丟bang，不要->扣血。 //直接丟到棄牌堆
void card_duel(sGame *pGame, i32 player_id, i32 card_id );    // 用while()，跑你、我、你、我，直到偵測到throw_card回傳1，就break。 //直接丟到棄牌堆

// table
void card_mustang(sGame *pGame, i32 player_id, i32 card_id ); // 丟到自己的桌上
void card_scope(sGame *pGame, i32 player_id, i32 card_id );   // 丟到自己的桌上
void card_barrel(sGame *pGame, i32 player_id, i32 card_id );  // 丟到自己的桌上
void card_jail(sGame *pGame, i32 player_id, i32 card_id );    // 丟到某人的桌上
void card_bomb(sGame *pGame, i32 player_id, i32 card_id );    // 丟到自己的桌上 

// guns
void card_remington(sGame *pGame, i32 player_id, i32 card_id ); // 丟到自己的桌上 
void card_schofild(sGame *pGame, i32 player_id, i32 card_id );  // 丟到自己的桌上 
void card_winchester(sGame *pGame, i32 player_id, i32 card_id );// 丟到自己的桌上 
void card_carabine(sGame *pGame, i32 player_id, i32 card_id );  // 丟到自己的桌上 
void card_volcano(sGame *pGame, i32 player_id, i32 card_id );   // 丟到自己的桌上 