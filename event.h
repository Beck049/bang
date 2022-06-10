#pragma once

#include "game.h"

#define MAX_ID 32
#define GAME_ID (MAX_ID-1)
#define EVENT_APPLY_FUNC(_pGame, _ptr_to_func, _event) ((*(void (*)(typeof(_pGame), typeof(&_event)))_ptr_to_func)(_pGame, &_event))

typedef struct _sTakeEvent {
	i32 src_id;
	sList *src;
	bool order;
	i32 take_res;
} sTakeEvent;

typedef struct _sDrawEvent {
	sList *draw_res;
} sDrawEvent;

typedef struct _sDamageEvent {
	i32 damage;
	i32 damager;
	i32 victim;
} sDamageEvent;

typedef struct _sDetermineEvent {
	i32 dtm_res;
} sDetermineEvent;

typedef struct _sDodgeEvent {
	bool dodge_res;
} sDodgeEvent;

typedef struct _sBangEvent {
	bool bang_res;
	i32 trigger_id;
	i32 target_id;
} sBangEvent;

typedef struct _sDeathEvent {
	i32 dead_id;
	i32 killer_id;
	// type (optional)
} sDeathEvent;

typedef struct _sSelectEvent {
	i32 min_cnt;
	i32 max_cnt;
	sList *selection;  // list of string to select
	sList *select_res;  // list of index selected
} sSelectEvent;

typedef struct _sLthalEvent {
	i32 lethal;
} sLethalEvent;

sList *take_event_funcs[MAX_ID];
sList *draw_event_funcs[MAX_ID];
sList *draw_phase_event_funcs[MAX_ID];
sList *damage_event_funcs[MAX_ID];
sList *dodge_event_funcs[MAX_ID];
sList *bang_event_funcs[MAX_ID];
sList *death_event_funcs[MAX_ID];
sList *lethal_event_funcs[MAX_ID];

// from `src` take 1 card
sTakeEvent take_card_event(sGame *pGame, sList *src, i32 src_id, bool order);

// give `cards` to `dst`
void give_card_event(sGame *pGame, sList *dst, i32 dst_id, sList *src);

// from `src` take the most `cnt` cards, and give the cards to `dst`
sDrawEvent draw_event(sGame *pGame, sList *dst, i32 dst_id, sList *src, i32 src_id, i32 cnt, i32 order);

void draw_phase_event(sGame *pGame, i32 target_id);

sDamageEvent damage_event(sGame *pGame, i32 victim_id, i32 damager_id, i32 damage);

sDodgeEvent dodge_event(sGame *pGame, i32 target_id);

sBangEvent bang_event(sGame *pGame, i32 target_id);

sDeathEvent death_event(sGame *pGame, i32 dead_id, i32 killer_id);

// player can select wether heal him self or not
sLethalEvent lethal_event(sGame *pGame, i32 target_id);