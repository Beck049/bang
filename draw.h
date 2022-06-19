#pragma once

#include "event.h"
#include "game_play.h"

void swap_pile(sList *pListA, sList *pListB);

i32 take_card(sGame *pGame, sList *src, i32 pos);

i32 take_card_by_id(sGame *pGame, sList *src, i32 card_id);

void give_card(sGame *pGame, sList *dst, i32 card_id, bool tail_insert);

void draw_phase_event_default(sGame *pGame, sDrawPhaseEvent *e);


void draw_phase_event_black_jack(sGame *pGame, sDrawPhaseEvent *e);
	
void draw_phase_event_jesse_jones(sGame *pGame, sDrawPhaseEvent *e);
	
void draw_phase_event_kit_carlson(sGame *pGame, sDrawPhaseEvent *e);
	
void draw_phase_event_pedro_ramirez(sGame *pGame, sDrawPhaseEvent *e);