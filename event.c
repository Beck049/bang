#include "event.h"

sList *draw_phase_event_funcs[MAX_ID];
sList *play_phase_event_funcs[MAX_ID];
sList *damage_event_funcs[MAX_ID];
sList *determine_event_funcs[MAX_ID];
sList *dodge_event_funcs[MAX_ID];
sList *bang_event_funcs[MAX_ID];
sList *death_event_funcs[MAX_ID];
sList *select_event_funcs[MAX_ID];
sList *lethal_event_funcs[MAX_ID];
sList **event_funcs[EVENT_TYPE_SIZ] = {
	draw_phase_event_funcs,
	play_phase_event_funcs,
	damage_event_funcs,
	determine_event_funcs,
	dodge_event_funcs,
	bang_event_funcs,
	death_event_funcs,
	select_event_funcs,
	lethal_event_funcs,
};

void init_event_funcs() {
	for(i32 i = 0; i < MAX_ID; ++i) {
		draw_phase_event_funcs[i] = new_list();
		play_phase_event_funcs[i] = new_list();
		damage_event_funcs[i] = new_list();
		determine_event_funcs[i] = new_list();
		dodge_event_funcs[i] = new_list();
		bang_event_funcs[i] = new_list();
		death_event_funcs[i] = new_list();
		select_event_funcs[i] = new_list();
		lethal_event_funcs[i] = new_list();
	}
}

void free_event_funcs() {
	for(i32 i = 0; i < MAX_ID; ++i) {
		free_list(draw_phase_event_funcs[i]);
		free_list(play_phase_event_funcs[i]);
		free_list(damage_event_funcs[i]);
		free_list(determine_event_funcs[i]);
		free_list(dodge_event_funcs[i]);
		free_list(bang_event_funcs[i]);
		free_list(death_event_funcs[i]);
		free_list(select_event_funcs[i]);
		free_list(lethal_event_funcs[i]);
	}
}

void register_event_func(eEventType type, i32 player_id, void *func) {
	void **data = malloc(sizeof(func));
	*data = func;
	list_push_front(event_funcs[type][player_id], new_node(data));
}

sDrawPhaseEvent draw_phase_event(sGame *pGame, i32 target_id) {
	sDrawPhaseEvent drw_ph_e = {
		.target_id = target_id,
		.draw_phase_res = new_list(),
	};
	LIST_FOR_EACH(pNode, draw_phase_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &drw_ph_e);
	}
	return drw_ph_e;
}

sPlayPhaseEvent play_phase_event(sGame *pGame, i32 target_id) {
	sPlayPhaseEvent play_ph_e = {
		.target_id = target_id,
		.result = false,
	};
	LIST_FOR_EACH(pNode, play_phase_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &play_ph_e);
		if(pGame->end_winner_role != (eRole)-1) break;
	}
	return play_ph_e;
}

sDamageEvent damage_event(sGame *pGame, i32 victim_id, i32 damager_id, i32 damage) {
	sDamageEvent dmg_e = {
		.victim_id = victim_id,
		.damager_id = damager_id,
		.damage = damage,
	};
	LIST_FOR_EACH(pNode, damage_event_funcs[victim_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &dmg_e);
		if(pGame->end_winner_role != (eRole)-1) break;
	}
	return dmg_e;
}

sDetermineEvent determine_event(sGame *pGame, i32 target_id) {
	sDetermineEvent dtm_e = {
		.target_id = target_id,
		.determine_res = -1,
	};
	LIST_FOR_EACH(pNode, dodge_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &dtm_e);
		if(pGame->end_winner_role != (eRole)-1) break;
		if(dtm_e.determine_res != -1) break;
	}
	return dtm_e;
}

sDodgeEvent dodge_event(sGame *pGame, i32 target_id, i32 dodge_times) {
	sDodgeEvent dg_e = {
		.target_id = target_id,
		.dodge_res = false,
		.dodge_times = dodge_times,
	};
	LIST_FOR_EACH(pNode, dodge_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &dg_e);
		if(pGame->end_winner_role != (eRole)-1) break;
		if(dg_e.dodge_res == true) break;
	}
	return dg_e;
}

sBangEvent bang_event(sGame *pGame, i32 trigger_id, i32 target_id) {
	sBangEvent bng_e = {
		.trigger_id = trigger_id,
		.target_id = target_id,
		.bang_res = false,
	};
	LIST_FOR_EACH(pNode, bang_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &bng_e);
		if(pGame->end_winner_role != (eRole)-1) break;
		if(bng_e.bang_res == true) break;
	}
	return bng_e;
}

sDeathEvent death_event(sGame *pGame, i32 dead_id, i32 killer_id) {
	sDeathEvent dth_e = {
		.dead_id = dead_id,
		.killer_id = killer_id,
		.death_res = false,
	};
	LIST_FOR_EACH(pNode, death_event_funcs[dead_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &dth_e);
		if(pGame->end_winner_role != (eRole)-1) break;
		if(dth_e.death_res == true) break;
	}
	return dth_e;
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
	while((str = va_arg(ap, char*)) != NULL) {
		list_push_back(sl_e.selections, new_node(strdup(str)));
	}
	va_end(ap);
	LIST_FOR_EACH(pNode, select_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &sl_e);
	}
	return sl_e;
}

sSelectEvent select_event_with_arr(sGame *pGame, i32 target_id, i32 min_cnt, i32 max_cnt, void *options, i32 optcnt, i32 optlen) {
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
	sLethalEvent lth_e = {
		.target_id = target_id,
		.lethal_res = true,
	};
	LIST_FOR_EACH(pNode, lethal_event_funcs[target_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, &lth_e);
		if(pGame->end_winner_role != (eRole)-1) break;
		if(lth_e.lethal_res == false) break;
	}
	return lth_e;
}

