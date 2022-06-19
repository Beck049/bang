#include "select.h"
#include "display.h"

void select_event_player(sGame *pGame, sSelectEvent *e) {
	display_selection(pGame, 0, *e);  // display to player 0

	char buf[BUFSIZ];
	i32 sl_cnt = 0;
	i32 size = e->selections->size;
	bool selected[size];
	memset(selected, false, sizeof(selected));
	
	while(sl_cnt < e->min_cnt || sl_cnt > e->max_cnt) {
		while(!fgets_n(buf, sizeof(buf), stdin));
		char *cur = buf;
		while(cur && *cur) {
			i32 x = strtol(cur, &cur, 10);
			--x;  // trnsfer to 0 index: 1, 2, 3 -> 0, 1, 2
			selected[x] = !selected[x];
		}
		sl_cnt = 0;
		for(i32 i = 0; i < size; ++i) {
			if(selected[i]) ++sl_cnt;
		}
		display_selected(pGame, 0, selected, size);
	}
	for(i32 i = 0; i < size; ++i) {
		if(selected[i]) {
			i32 *idx = malloc(sizeof(i32));
			*idx = i;
			list_push_back(e->select_res, new_node(idx));
		}
	}
}

void select_event_bot(sGame *pGame, sSelectEvent *e) {
	// display_selection(pGame, 0, *e);  // display to player 0

	i32 size = e->selections->size;
	sList *res = e->select_res;
	bool selected[size];
	memset(selected, false, sizeof(selected));

	// init result with [0, 1, 2, ...,`size`] and shuffle it
	list_init(res, size);
	shuffle(res);

	// get first `min_cnt` selection
	i32 sl_cnt = 0;
	LIST_FOR_EACH(pNode, res) {
		if(sl_cnt == e->min_cnt) break;
		i32 idx = *(i32*)pNode->data;
		selected[idx] = true;
		++sl_cnt;
	}
	
	// display_selected(pGame, 0, selected, size);
}

i32 select_player(sGame *pGame, i32 trigger_id) {
	sList *live_players = pGame->live_players;
	i32 size = live_players->size;
	i32 players_id[size-1];
	char options[size-1][16];

	sListNode *pCurPlayer = get_player(pGame, trigger_id);
	pCurPlayer = get_next_player(pGame, pCurPlayer);  // skip self
	for(i32 i = 0; i < size-1; ++i) {
		i32 id = *(i32*)pCurPlayer->data;
		players_id[i] = id;
		sprintf(options[i], "%2d) player%d", i+1, id);
		pCurPlayer = get_next_player(pGame, pCurPlayer);
	}
	sSelectEvent sl_e = select_event_with_arr(pGame, trigger_id, 1, 1, options, size-1, sizeof(*options));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	free_list(sl_e.select_res);
	free_list(sl_e.selections);
	return players_id[select_idx];
}