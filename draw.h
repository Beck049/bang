#pragma once

#include "event.h"

void swap_pile(sList *pListA, sList *pListB);

i32 take_card(sGame *pGame, sList *src, i32 pos);

void give_card(sGame *pGame, sList *dst, i32 card_id, bool tail_insert);

void draw_phase_event_default(sGame *pGame, sDrawPhaseEvent *e);