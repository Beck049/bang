#include "event.h"

sTakeEvent take_card_event(sGame *pGame, sList *src, i32 src_id, bool order) {
	sTakeEvent tk_e = {
		.src_id = src_id,
		.src = src,
		.order = order,
		.take_res = -1,
	};
	LIST_FOR_EACH(pNode, take_event_funcs[src_id]) {
		EVENT_APPLY_FUNC(pGame, pNode->data, tk_e);
	}
	return tk_e;
}

void give_card_event(sGame *pGame, sList *dst, i32 dst_id, sList *src) {

}

sDrawEvent draw_event(sGame *pGame, sList *dst, i32 dst_id, sList *src, i32 src_id, i32 cnt, i32 order) {

}

void draw_phase_event(sGame *pGame, i32 target_id) {

}

sDamageEvent damage_event(sGame *pGame, i32 victim_id, i32 damager_id, i32 damage) {

}

sDodgeEvent dodge_event(sGame *pGame, i32 target_id) {

}

sBangEvent bang_event(sGame *pGame, i32 target_id) {

}

sDeathEvent death_event(sGame *pGame, i32 dead_id, i32 killer_id) {

}

sLethalEvent lethal_event(sGame *pGame, i32 target_id) {

}