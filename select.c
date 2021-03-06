#include "select.h"
#include "display.h"

void select_event_player(sGame *pGame, sSelectEvent *e) {
	display_selection(pGame, 0, *e);  // display to player 0

	char buf[BUFSIZ];
	i32 sl_cnt = 0;
	i32 size = e->selections->size;
	bool selected[size];
	memset(selected, false, sizeof(selected));
	
	while(true) {
		printf(LCYN);
		while(!fgets_n(buf, sizeof(buf), stdin));
		printf(RST);

		char *cur = buf;
		while(cur) {
			cur += strspn(cur, " ");
			if(*cur == '\0') break;

			if(!isdigit(*cur)) {
				printf("非法輸入: %s\n", cur);
				break;
			}
			i32 x = strtol(cur, &cur, 10);
			if(x < 1 || x > size) {
				printf("索引值 %d 是個非法輸入!\n", x);
				break;
			}
			--x;  // trnsfer to 0 index: 1, 2, 3 -> 0, 1, 2
			selected[x] = !selected[x];
		}
		sl_cnt = 0;
		for(i32 i = 0; i < size; ++i) {
			if(selected[i]) ++sl_cnt;
		}
		display_selected(pGame, 0, selected, size);
		printf("\n");

		if(sl_cnt < e->min_cnt || sl_cnt > e->max_cnt) continue;

		printf("選擇成功!\n\n");
		break;
	}
	for(i32 i = 0; i < size; ++i) {
		if(selected[i]) {
			i32 *idx = malloc(sizeof(i32));
			*idx = i;
			list_push_back(e->select_res, new_node(idx));
		}
	}
}

void select_event_bot(__attribute__((unused)) sGame *pGame, sSelectEvent *e) {
	// display_selection(pGame, 0, *e);  // display to player 0
	printf("玩家 %d 正在做出抉擇...\n", e->target_id);
	i32 size = e->selections->size;
	sList *res = e->select_res;
	bool selected[size];
	memset(selected, false, sizeof(selected));

	// init result with [0, 1, 2, ...,`size`] and shuffle it
	list_init(res, size);
	shuffle(res);

	while((i32)res->size > e->min_cnt) {
		take_card(pGame, res, 0);
	}

	// get first `min_cnt` selection
	// i32 sl_cnt = 0;
	// LIST_FOR_EACH(pNode, res) {
	// 	if(sl_cnt == e->min_cnt) break;
	// 	i32 idx = *(i32*)pNode->data;
	// 	selected[idx] = true;
	// 	++sl_cnt;
	// }
	
	// display_selected(pGame, 0, selected, size);
}

i32 select_player(sGame *pGame, i32 trigger_id) {
	sList *live_players = pGame->live_players;
	i32 size = live_players->size;
	i32 players_id[size-1];
	char options[size-1][512];

	sListNode *pCurPlayer = get_player(pGame, trigger_id);
	pCurPlayer = get_next_player(pGame, pCurPlayer);  // skip self
	for(i32 i = 0; i < size-1; ++i) {
		i32 id = *(i32*)pCurPlayer->data;
		players_id[i] = id;
		sprintf(options[i], "%2d) 玩家 %d", i+1, id);
		pCurPlayer = get_next_player(pGame, pCurPlayer);
	}
	sSelectEvent sl_e = select_event_with_arr(pGame, trigger_id, 1, 1, options, size-1, sizeof(*options));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	free_list(sl_e.select_res);
	free_list(sl_e.selections);
	return players_id[select_idx];
}

i32 select_throw(sGame *pGame, i32 target_id, eCardType card_type) {
	sPlayer *pTarget = &pGame->players[target_id];
	i32 size = pTarget->cards->size;

	if(size == 0) return -1;

	i32 cnt = 0;
	i32 hands_id[size];
	char options[size][512];

	sListNode *pCurNode = LIST_BEGIN(pTarget->cards);
	for(i32 i = 0; i < size; ++i) {
		i32 id = *(i32*)pCurNode->data;
		sCard *card = &cards[id];
		static const int JANET = 3;
		// is janet and card_type is miss or bang
		if(pTarget->character == JANET
			&& (card_type == MISS || card_type == BANG)) {
			if(card->type == MISS || card->type == BANG) {
				hands_id[cnt] = id;
				sprintf(options[cnt], "%2d) %s", cnt+1, card->name);
				++cnt;
			}
		}
		else {
			if(card->type == card_type) {
				hands_id[cnt] = id;
				sprintf(options[cnt], "%2d) %s", cnt+1, card->name);
				++cnt;
			}
		}
	}

	if(cnt == 0) return -1;
	
	sSelectEvent sl_e = select_event_with_arr(pGame, target_id, 1, 1, options, cnt, sizeof(*options));
	i32 select_idx = *(i32*)LIST_FRONT(sl_e.select_res);
	free_list(sl_e.select_res);
	free_list(sl_e.selections);
	return hands_id[select_idx];
}