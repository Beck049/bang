#pragma once

#include "basic.h"
#include "list.h"
#include "init_card.h"
#include "game_play.h"
#define ASCII_ESC "\033"
#define RST ASCII_ESC"[0m"
#define RED ASCII_ESC"[31m"
#define GRN ASCII_ESC"[32m"
#define YLW ASCII_ESC"[33m"
#define BL ASCII_ESC"[34m" 
#define MGT ASCII_ESC"[35m"
#define CYN ASCII_ESC"[36m"
#define WHT ASCII_ESC"[37m"
#define GRY ASCII_ESC"[90m"
#define LRED ASCII_ESC"[91m"
#define LGRN ASCII_ESC"[92m"
#define LYLW ASCII_ESC"[93m"
#define LBL ASCII_ESC"[94m"
#define LMGT ASCII_ESC"[95m"
#define LCYN ASCII_ESC"[96m"
#define LWHT ASCII_ESC"[97m"

void print_card(char *dst, i32 card_id);

void display_pile(sList *pList);

void display_game(sGame *pGame, i32 viewer_id);

void display_damage(sGame *pGame, i32 viewer_id, sDamageEvent e);

void display_has_bomb(sGame *pGame, i32 viewer_id, i32 bomb_owner_id);

void display_determine(sGame *pGame, i32 viewer_id, i32 draw_num);

void display_bang(sGame *pGame, i32 viewer_id, sBangEvent e);

void display_selection(sGame *pGame, i32 viewer_id, sSelectEvent e);

void display_selected(sGame *pGame, i32 viewer_id, bool *selected, i32 size);

void display_end(sGame *pGame, eRole role);

void start_style();

void color();