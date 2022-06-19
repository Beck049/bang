#pragma once

#include "game.h"

#define MAX_ID 32
#define GAME_ID (MAX_ID-1)
#define EVENT_APPLY_FUNC(_pGame, _pToFunc, _pEvent) ((*((void (**)(typeof(_pGame), typeof(_pEvent)))(_pToFunc)))(_pGame, _pEvent))
#define EVENT_TYPE_SIZ 9

typedef enum _eEventType {
	EVENT_DRAW_PHASE = 0,
	EVENT_PLAY_PHASE,
	EVENT_DAMAGE,
	EVENT_DETERMINE,
	EVENT_DODGE,
	EVENT_BANG,
	EVENT_DEATH,
	EVENT_SELECT,
	EVENT_LETHAL,
} eEventType;

typedef struct _sDrawPhaseEvent {
	i32 target_id;
	sList *draw_phase_res;
} sDrawPhaseEvent;

typedef struct _PlayPhaseEvent {
	i32 target_id;
	bool result;
} sPlayPhaseEvent;

typedef struct _sDamageEvent {
	i32 damager_id;
	i32 victim_id;
	i32 damage;
} sDamageEvent;

typedef struct _sDetermineEvent {
	i32 target_id;
	i32 determine_res;
} sDetermineEvent;

typedef struct _sDodgeEvent {
	i32 target_id;
	bool dodge_res;
	i32 dodge_times;
} sDodgeEvent;

typedef struct _sBangEvent {
	i32 trigger_id;
	i32 target_id;
	bool bang_res;
} sBangEvent;

typedef struct _sDeathEvent {
	i32 dead_id;
	i32 killer_id;
	bool death_res;
	// type (optional)
} sDeathEvent;

typedef struct _sSelectEvent {
	i32 min_cnt;
	i32 max_cnt;
	sList *selections;  // list of string to select
	sList *select_res;  // list of index selected
} sSelectEvent;

typedef struct _sLethalEvent {
	i32 target_id;
	bool lethal_res;
} sLethalEvent;

extern sList *draw_phase_event_funcs[MAX_ID];
extern sList *play_phase_event_funcs[MAX_ID];
extern sList *damage_event_funcs[MAX_ID];
extern sList *determine_event_funcs[MAX_ID];
extern sList *dodge_event_funcs[MAX_ID];
extern sList *bang_event_funcs[MAX_ID];
extern sList *death_event_funcs[MAX_ID];
extern sList *select_event_funcs[MAX_ID];
extern sList *lethal_event_funcs[MAX_ID];
extern sList **event_funcs[EVENT_TYPE_SIZ];

void init_event_funcs();

void free_event_funcs();

void register_event_func(eEventType type, i32 player_id, void *func);

sDrawPhaseEvent draw_phase_event(sGame *pGame, i32 target_id);

sPlayPhaseEvent play_phase_event(sGame *pGame, i32 target_id);

sDamageEvent damage_event(sGame *pGame, i32 victim_id, i32 damager_id, i32 damage);

sDetermineEvent determine_event(sGame *pGame, i32 target_id);

sDodgeEvent dodge_event(sGame *pGame, i32 target_id, i32 dodge_times);

sBangEvent bang_event(sGame *pGame, i32 trigger_id, i32 target_id);

sDeathEvent death_event(sGame *pGame, i32 dead_id, i32 killer_id);

sSelectEvent select_event(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, ...);

sSelectEvent select_event_with_arr(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, void *options, i32 optcnt, i32 optlen);

// player can select wether heal him self or not
sLethalEvent lethal_event(sGame *pGame, i32 target_id);