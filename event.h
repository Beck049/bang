#pragma once

#include "game.h"

#define MAX_ID 32
#define GAME_ID (MAX_ID-1)
#define EVENT_APPLY_FUNC(_pGame, _pToFunc, _pEvent) ((*((void (**)(typeof(_pGame), typeof(_pEvent)))(_pToFunc)))(_pGame, _pEvent))

typedef struct _sDrawPhaseEvent {
	i32 target_id;
    sList *draw_phase_res;
} sDrawPhaseEvent;

typedef struct _sDamageEvent {
	i32 damage;
	i32 damager;
	i32 victim;
} sDamageEvent;

typedef struct _sDetermineEvent {
	i32 target_id;
	i32 determine_res;
} sDetermineEvent;

typedef struct _sDodgeEvent {
	i32 target_id;
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
	sList *selections;  // list of string to select
	sList *select_res;  // list of index selected
} sSelectEvent;

typedef struct _sLthalEvent {
	i32 lethal;
} sLethalEvent;

extern sList *draw_phase_event_funcs[MAX_ID];
extern sList *damage_event_funcs[MAX_ID];
extern sList *dodge_event_funcs[MAX_ID];
extern sList *bang_event_funcs[MAX_ID];
extern sList *death_event_funcs[MAX_ID];
extern sList *select_event_funcs[MAX_ID];
extern sList *lethal_event_funcs[MAX_ID];

sDamageEvent draw_phase_event(sGame *pGame, i32 target_id);

sDamageEvent damage_event(sGame *pGame, i32 victim_id, i32 damager_id, i32 damage);

sDetermineEvent determine_event(sGame *pGame, i32 target_id);

sDodgeEvent dodge_event(sGame *pGame, i32 target_id);

sBangEvent bang_event(sGame *pGame, i32 target_id);

sDeathEvent death_event(sGame *pGame, i32 dead_id, i32 killer_id);

sSelectEvent select_event(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, ...);

sSelectEvent select_event_with_arr(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, void *options, i32 optcnt, i32 optlen);

// player can select wether heal him self or not
sLethalEvent lethal_event(sGame *pGame, i32 target_id);