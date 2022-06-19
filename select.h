#pragma once

#include "event.h"

void select_event_player(sGame *pGame, sSelectEvent *e);

void select_event_bot(sGame *pGame, sSelectEvent *e);

// select a player except self
i32 select_player(sGame *pGame, i32 trigger_id);

// select card with type and throw it
i32 select_throw(sGame *pGame, i32 target_id, eCardType card_type);