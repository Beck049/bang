#pragma once

#include "basic.h"
#include "list.h"
#include "init_card.h"
#include "game_play.h"

void print_card(char *dst, i32 card_id);

void display_pile(sList *pList);

void display_game(sGame *pGame, i32 viewer_id);

void display_damage(sGame *pGame, i32 viewer_id, sDamageEvent e);

void display_has_bomb(sGame *pGame, i32 viewer_id, i32 bomb_owner_id);

void display_determine(sGame *pGame, i32 viewer_id, i32 draw_num);