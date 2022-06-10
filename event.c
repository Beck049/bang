#include "event.h"

sList *take_event_funcs[MAX_ID];
sList *draw_event_funcs[MAX_ID];
sList *draw_phase_event_funcs[MAX_ID];
sList *damage_event_funcs[MAX_ID];
sList *dodge_event_funcs[MAX_ID];
sList *bang_event_funcs[MAX_ID];
sList *death_event_funcs[MAX_ID];
sList *select_event_funcs[MAX_ID];
sList *lethal_event_funcs[MAX_ID];

sDamageEvent draw_phase_event(sGame *pGame, i32 target_id) {
	sDrawPhaseEvent drw_ph_e = {
		.target_id = target_id,
		.draw_phase_res = new_list(),
	};
	LIST_FOR_EACH(pNode, draw_phase_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &drw_ph_e);
	}
}

sDamageEvent damage_event(sGame *pGame, i32 victim_id, i32 damager_id, i32 damage) {

}

sDodgeEvent dodge_event(sGame *pGame, i32 target_id) {

}

sBangEvent bang_event(sGame *pGame, i32 target_id) {

}

sDeathEvent death_event(sGame *pGame, i32 dead_id, i32 killer_id) {

}

sSelectEvent select_event(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, ...) {
	const char *str;
	sSelectEvent sl_e = {
		.min_cnt = min_cnt,
		.max_cnt = max_cnt,
		.selections = new_list(),  // list of string to select
		.select_res = new_list(),  // list of index selected
	};
	va_list ap;
	va_start(ap, max_cnt);
	while(str = va_arg(ap, char*)) {
		list_push_back(sl_e.selections, new_node(strdup(str)));
	}
	va_end(ap);
	LIST_FOR_EACH(pNode, select_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &sl_e);
	}
	return sl_e;
}

sSelectEvent select_event_with_arr(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, void *options, i32 optcnt, i32 optlen) {
	const char *str;
	sSelectEvent sl_e = {
		.min_cnt = min_cnt,
		.max_cnt = max_cnt,
		.selections = new_list(),  // list of string to select
		.select_res = new_list(),  // list of index selected
	};
	for(i32 i = 0; i < optcnt; ++i) {
		list_push_back(sl_e.selections, new_node(strdup(options+i*optlen)));
	}
	LIST_FOR_EACH(pNode, select_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &sl_e);
	}
	return sl_e;
}

sLethalEvent lethal_event(sGame *pGame, i32 target_id) {

}