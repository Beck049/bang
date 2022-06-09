#pragma once

#include "basic.h"
#include "list.h"

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

// from `src` take `cnt` cards
sList *take_card_event(i32 src_id, i32 cnt);

// give `cards` to `dst`
void give_card_event(i32 dst_id, sList *cards);

// from `src` take the most `cnt` cards, and give the cards to `dst`
sDrawEvent draw_event(i32 dst_id, i32 src_id, i32 cnt);

void draw_phase_event(i32 target_id);

sDamageEvent damage_event(i32 victim_id, i32 damager_id, i32 damage);

sDodgeEvent dodge_event(i32 target_id);

sBangEvent bang_event(i32 target_id);

// player can select wether heal him self or not
sLethalEvent lethal_event(i32 target_id);